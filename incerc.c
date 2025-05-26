#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1000
#define MAX_CHANGES 10000

// Structure for coordinates
typedef struct {
    int row, col;
} Coord;

// Structure for stack element (Task 2)
typedef struct {
    int generation;
    Coord changes[MAX_CHANGES];
    int num_changes;
} StackElement;

// Structure for tree node (Task 3)
typedef struct TreeNode {
    Coord changes[MAX_CHANGES];
    int num_changes;
    int is_rule_b;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// Global variables
char grid[MAX_SIZE][MAX_SIZE];
char new_grid[MAX_SIZE][MAX_SIZE];
int n, m, k;

// Function prototypes
int count_neighbors(int i, int j);
int apply_standard_rules(Coord changes[]);
int apply_rule_b(Coord changes[]);
void copy_grid(char dest[MAX_SIZE][MAX_SIZE], char src[MAX_SIZE][MAX_SIZE]);
int get_live_cells(Coord cells[]);
TreeNode* create_node();
TreeNode* build_tree(Coord initial_live[], int num_initial);
void preorder_traversal(TreeNode* root, Coord initial_live[], int num_initial, 
                       char matrices[][MAX_SIZE][MAX_SIZE], int* matrix_count);
void reconstruct_grid_from_changes(char result[MAX_SIZE][MAX_SIZE], 
                                  char parent[MAX_SIZE][MAX_SIZE], 
                                  Coord changes[], int num_changes);
int create_adjacency_matrix(char grid[MAX_SIZE][MAX_SIZE], 
                           int adj_matrix[MAX_SIZE][MAX_SIZE], 
                           Coord live_cells[], int* num_live);
int find_hamiltonian_path(int adj_matrix[MAX_SIZE][MAX_SIZE], 
                         Coord live_cells[], int num_live, 
                         Coord path[]);
void dfs_hamiltonian(int adj_matrix[MAX_SIZE][MAX_SIZE], int num_nodes,
                    int current_path[], int path_length, int visited[],
                    int* best_length, int best_path[]);

// Task functions
void task1();
void task2();
void task3();
void task4();

int main() {
    int task;
    char line[MAX_SIZE];
    
    // Read input
    scanf("%d", &task);
    scanf("%d %d", &n, &m);
    scanf("%d", &k);
    
    // Read grid
    for (int i = 0; i < n; i++) {
        scanf("%s", grid[i]);
    }
    
    // Execute appropriate task
    switch (task) {
        case 1: task1(); break;
        case 2: task2(); break;
        case 3: task3(); break;
        case 4: task4(); break;
        default: printf("Invalid task number\n"); break;
    }
    
    return 0;
}

int count_neighbors(int i, int j) {
    int count = 0;
    int directions[8][2] = {{-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1}};
    
    for (int d = 0; d < 8; d++) {
        int ni = i + directions[d][0];
        int nj = j + directions[d][1];
        
        if (ni >= 0 && ni < n && nj >= 0 && nj < m && grid[ni][nj] == 'X') {
            count++;
        }
    }
    return count;
}

int apply_standard_rules(Coord changes[]) {
    int change_count = 0;
    
    // Initialize new grid
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            new_grid[i][j] = '+';
        }
    }
    
    // Apply rules
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int neighbors = count_neighbors(i, j);
            char current = grid[i][j];
            
            if (current == 'X') {  // Live cell
                if (neighbors == 2 || neighbors == 3) {
                    new_grid[i][j] = 'X';  // Survives
                }
                // else dies (already set to '+')
            } else {  // Dead cell
                if (neighbors == 3) {
                    new_grid[i][j] = 'X';  // Born
                }
                // else stays dead (already set to '+')
            }
            
            // Track changes
            if (new_grid[i][j] != current) {
                changes[change_count].row = i;
                changes[change_count].col = j;
                change_count++;
            }
        }
    }
    
    // Copy new grid to current grid
    copy_grid(grid, new_grid);
    return change_count;
}

int apply_rule_b(Coord changes[]) {
    int change_count = 0;
    
    // Initialize new grid
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            new_grid[i][j] = '+';
        }
    }
    
    // Apply rule B: exactly 2 neighbors -> alive
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int neighbors = count_neighbors(i, j);
            char current = grid[i][j];
            
            if (neighbors == 2) {
                new_grid[i][j] = 'X';
            } else {
                new_grid[i][j] = '+';
            }
            
            // Track changes
            if (new_grid[i][j] != current) {
                changes[change_count].row = i;
                changes[change_count].col = j;
                change_count++;
            }
        }
    }
    
    // Copy new grid to current grid
    copy_grid(grid, new_grid);
    return change_count;
}

void copy_grid(char dest[MAX_SIZE][MAX_SIZE], char src[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

int get_live_cells(Coord cells[]) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == 'X') {
                cells[count].row = i;
                cells[count].col = j;
                count++;
            }
        }
    }
    return count;
}

void task1() {
    for (int gen = 0; gen < k; gen++) {
        Coord changes[MAX_CHANGES];
        apply_standard_rules(changes);
        
        // Print current generation
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                printf("%c", grid[i][j]);
            }
            printf("\n");
        }
        if (gen < k - 1) printf("\n");
    }
}

void task2() {
    StackElement stack[MAX_SIZE];
    int stack_size = 0;
    
    for (int gen = 1; gen <= k; gen++) {
        Coord changes[MAX_CHANGES];
        int num_changes = apply_standard_rules(changes);
        
        stack[stack_size].generation = gen;
        stack[stack_size].num_changes = num_changes;
        for (int i = 0; i < num_changes; i++) {
            stack[stack_size].changes[i] = changes[i];
        }
        stack_size++;
    }
    
    // Output stack contents
    for (int i = 0; i < stack_size; i++) {
        printf("%d", stack[i].generation);
        for (int j = 0; j < stack[i].num_changes; j++) {
            printf(" (%d,%d)", stack[i].changes[j].row, stack[i].changes[j].col);
        }
        printf("\n");
    }
}

TreeNode* create_node() {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->num_changes = 0;
    node->is_rule_b = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}

TreeNode* build_tree(Coord initial_live[], int num_initial) {
    TreeNode* root = create_node();
    
    // Root contains initial live cells
    for (int i = 0; i < num_initial; i++) {
        root->changes[i] = initial_live[i];
    }
    root->num_changes = num_initial;
    
    // Queue for BFS tree construction
    TreeNode* queue[MAX_SIZE * 10];
    char grid_states[MAX_SIZE * 10][MAX_SIZE][MAX_SIZE];
    int levels[MAX_SIZE * 10];
    int front = 0, rear = 0;
    
    queue[rear] = root;
    copy_grid(grid_states[rear], grid);
    levels[rear] = 0;
    rear++;
    
    while (front < rear) {
        TreeNode* current = queue[front];
        char current_grid[MAX_SIZE][MAX_SIZE];
        copy_grid(current_grid, grid_states[front]);
        int level = levels[front];
        front++;
        
        if (level < k) {
            // Left child: Rule B
            copy_grid(grid, current_grid);
            Coord changes_b[MAX_CHANGES];
            int num_changes_b = apply_rule_b(changes_b);
            
            current->left = create_node();
            current->left->is_rule_b = 1;
            current->left->num_changes = num_changes_b;
            for (int i = 0; i < num_changes_b; i++) {
                current->left->changes[i] = changes_b[i];
            }
            
            queue[rear] = current->left;
            copy_grid(grid_states[rear], grid);
            levels[rear] = level + 1;
            rear++;
            
            // Right child: Standard rules
            copy_grid(grid, current_grid);
            Coord changes_std[MAX_CHANGES];
            int num_changes_std = apply_standard_rules(changes_std);
            
            current->right = create_node();
            current->right->is_rule_b = 0;
            current->right->num_changes = num_changes_std;
            for (int i = 0; i < num_changes_std; i++) {
                current->right->changes[i] = changes_std[i];
            }
            
            queue[rear] = current->right;
            copy_grid(grid_states[rear], grid);
            levels[rear] = level + 1;
            rear++;
        }
    }
    
    return root;
}

void reconstruct_grid_from_changes(char result[MAX_SIZE][MAX_SIZE], 
                                  char parent[MAX_SIZE][MAX_SIZE], 
                                  Coord changes[], int num_changes) {
    copy_grid(result, parent);
    for (int i = 0; i < num_changes; i++) {
        int r = changes[i].row;
        int c = changes[i].col;
        if (result[r][c] == 'X') {
            result[r][c] = '+';
        } else {
            result[r][c] = 'X';
        }
    }
}

void preorder_traversal(TreeNode* root, Coord initial_live[], int num_initial, 
                       char matrices[][MAX_SIZE][MAX_SIZE], int* matrix_count) {
    if (!root) return;
    
    // Stack for iterative preorder traversal
    TreeNode* node_stack[MAX_SIZE * 10];
    char grid_stack[MAX_SIZE * 10][MAX_SIZE][MAX_SIZE];
    int level_stack[MAX_SIZE * 10];
    int stack_top = 0;
    
    // Initialize with root
    node_stack[0] = root;
    // Set initial grid for root
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            grid_stack[0][i][j] = '+';
        }
    }
    for (int i = 0; i < num_initial; i++) {
        grid_stack[0][initial_live[i].row][initial_live[i].col] = 'X';
    }
    level_stack[0] = 0;
    
    while (stack_top >= 0) {
        TreeNode* current = node_stack[stack_top];
        char current_grid[MAX_SIZE][MAX_SIZE];
        copy_grid(current_grid, grid_stack[stack_top]);
        int level = level_stack[stack_top];
        stack_top--;
        
        // Add current matrix to result
        copy_grid(matrices[*matrix_count], current_grid);
        (*matrix_count)++;
        
        // Add children to stack (right first, then left for correct preorder)
        if (level < k) {
            if (current->right) {
                stack_top++;
                node_stack[stack_top] = current->right;
                reconstruct_grid_from_changes(grid_stack[stack_top], current_grid, 
                                            current->right->changes, current->right->num_changes);
                level_stack[stack_top] = level + 1;
            }
            
            if (current->left) {
                stack_top++;
                node_stack[stack_top] = current->left;
                reconstruct_grid_from_changes(grid_stack[stack_top], current_grid, 
                                            current->left->changes, current->left->num_changes);
                level_stack[stack_top] = level + 1;
            }
        }
    }
}

void task3() {
    // Get initial live cells
    Coord initial_live[MAX_CHANGES];
    int num_initial = get_live_cells(initial_live);
    
    // Build tree
    TreeNode* root = build_tree(initial_live, num_initial);
    
    // Get matrices from preorder traversal
    char matrices[MAX_SIZE * 10][MAX_SIZE][MAX_SIZE];
    int matrix_count = 0;
    preorder_traversal(root, initial_live, num_initial, matrices, &matrix_count);
    
    // Output matrices
    for (int mat = 0; mat < matrix_count; mat++) {
        if (mat > 0) printf("\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                printf("%c", matrices[mat][i][j]);
            }
            printf("\n");
        }
    }
}

int create_adjacency_matrix(char current_grid[MAX_SIZE][MAX_SIZE], 
                           int adj_matrix[MAX_SIZE][MAX_SIZE], 
                           Coord live_cells[], int* num_live) {
    *num_live = 0;
    int cell_to_index[MAX_SIZE][MAX_SIZE];
    
    // Initialize mapping
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cell_to_index[i][j] = -1;
        }
    }
    
    // Find live cells and create mapping
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (current_grid[i][j] == 'X') {
                live_cells[*num_live].row = i;
                live_cells[*num_live].col = j;
                cell_to_index[i][j] = *num_live;
                (*num_live)++;
            }
        }
    }
    
    if (*num_live == 0) return 0;
    
    // Initialize adjacency matrix
    for (int i = 0; i < *num_live; i++) {
        for (int j = 0; j < *num_live; j++) {
            adj_matrix[i][j] = 0;
        }
    }
    
    // Fill adjacency matrix
    int directions[8][2] = {{-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1}};
    
    for (int idx = 0; idx < *num_live; idx++) {
        int r1 = live_cells[idx].row;
        int c1 = live_cells[idx].col;
        
        for (int d = 0; d < 8; d++) {
            int r2 = r1 + directions[d][0];
            int c2 = c1 + directions[d][1];
            
            if (r2 >= 0 && r2 < n && c2 >= 0 && c2 < m && 
                current_grid[r2][c2] == 'X') {
                int neighbor_idx = cell_to_index[r2][c2];
                adj_matrix[idx][neighbor_idx] = 1;
            }
        }
    }
    
    return 1;
}

void dfs_hamiltonian(int adj_matrix[MAX_SIZE][MAX_SIZE], int num_nodes,
                    int current_path[], int path_length, int visited[],
                    int* best_length, int best_path[]) {
    if (path_length > *best_length) {
        *best_length = path_length;
        for (int i = 0; i < path_length; i++) {
            best_path[i] = current_path[i];
        }
    }
    
    if (path_length == 0) return;
    
    int current = current_path[path_length - 1];
    for (int next = 0; next < num_nodes; next++) {
        if (adj_matrix[current][next] && !visited[next]) {
            visited[next] = 1;
            current_path[path_length] = next;
            dfs_hamiltonian(adj_matrix, num_nodes, current_path, path_length + 1, 
                           visited, best_length, best_path);
            visited[next] = 0;
        }
    }
}

int find_hamiltonian_path(int adj_matrix[MAX_SIZE][MAX_SIZE], 
                         Coord live_cells[], int num_live, 
                         Coord path[]) {
    if (num_live == 0) return -1;
    
    int best_length = -1;
    int best_path[MAX_SIZE];
    
    // Try starting from each node (prioritized by coordinates)
    for (int start = 0; start < num_live; start++) {
        int visited[MAX_SIZE] = {0};
        int current_path[MAX_SIZE];
        
        visited[start] = 1;
        current_path[0] = start;
        dfs_hamiltonian(adj_matrix, num_live, current_path, 1, visited, 
                       &best_length, best_path);
    }
    
    if (best_length > 0) {
        for (int i = 0; i < best_length; i++) {
            path[i] = live_cells[best_path[i]];
        }
    }
    
    return best_length;
}

void task4() {
    // Get initial live cells
    Coord initial_live[MAX_CHANGES];
    int num_initial = get_live_cells(initial_live);
    
    // Build tree
    TreeNode* root = build_tree(initial_live, num_initial);
    
    // Get all grids from tree
    char matrices[MAX_SIZE * 10][MAX_SIZE][MAX_SIZE];
    int matrix_count = 0;
    preorder_traversal(root, initial_live, num_initial, matrices, &matrix_count);
    
    // Find Hamiltonian paths for each grid
    for (int mat = 0; mat < matrix_count; mat++) {
        int adj_matrix[MAX_SIZE][MAX_SIZE];
        Coord live_cells[MAX_CHANGES];
        int num_live;
        
        if (create_adjacency_matrix(matrices[mat], adj_matrix, live_cells, &num_live)) {
            Coord path[MAX_SIZE];
            int length = find_hamiltonian_path(adj_matrix, live_cells, num_live, path);
            
            printf("%d\n", length);
            if (length > 0) {
                for (int i = 0; i < length; i++) {
                    if (i > 0) printf(" ");
                    printf("(%d,%d)", path[i].row, path[i].col);
                }
                printf("\n");
            }
        } else {
            printf("-1\n");
        }
    }
}