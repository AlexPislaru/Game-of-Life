#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char** aloc_din_mat(int n,int m)
{
char **a= (char **) malloc(n * sizeof(char*));
if(a==NULL)
{
    printf("Eroare la alocare!\n");
    exit(1);
}
for (int i=0;i<n;i++)
{
    a[i]=(char *)malloc((m+1)*sizeof(char));
    if(a[i]==NULL)
    {
        printf("Eroare la alocarea liniei\n");
        exit(1);
    }
    a[i][0]='\0';
}
return a;
}

void free_mem(char ***a,int n)
{   
    if (*a==NULL) return;
    for(int i=0;i<n;i++)
    {
        free((*a)[i]);
    }
    free(*a);
    *a=NULL;
}

char** citire_mat(const char *intrare, int *n, int *m, int *g, int *t)
{
 FILE *fisier= fopen(intrare, "r");
 if(fisier==NULL)
 {
    printf("Eroare la deschiderea fisierului\n");
    exit(1);
 }
 fscanf(fisier,"%d",t);
 fscanf(fisier,"%d %d",n,m);
 fscanf(fisier,"%d", g);
 char **a= aloc_din_mat(*n,*m);
 for(int i=0;i< *n;i++)
 {
    fscanf(fisier,"%100s", a[i]);
 }
 fclose(fisier);
 return a;

}

int numar_vec(char **a,int n,int m, int x, int y)
{
    const int z[] = {-1,-1,-1,0,0,1,1,1};
    const int w[] = {-1,0,1,-1,1,-1,0,1};
    int nr=0;
    for(int i=0;i<8;i++)
    {
        int cx=x + z[i];
        int cy=y + w[i];
        if(cx>=0 && cx <n && cy >= 0 && cy < m && a[cx][cy]=='X')
        {
            nr++;
        }

    }
    return nr;

}

void act_mat(char **a,int n,int m)
{
char **b=aloc_din_mat(n,m);
for(int i=0;i<n;i++)
{
    for(int j=0;j<m;j++)
    {
        int v=numar_vec(a,n,m,i,j);
        if(a[i][j] == 'X')
        {
            if( v==2 || v==3)
            {
                b[i][j]='X';
            }else {
                b[i][j]= '+';
            }
        } else{
            if (v==3){
                b[i][j]='X';
            }else{
                b[i][j]= '+';
            }
        }
    }
}
for (int i = 0; i < n; i++) {
    for (int j=0;j<m;j++)
    {
        a[i][j]=b[i][j];
    }
    
}
free_mem(&b,n);
}

void scriere_mat(char **a,int n, const char *iesire)
{
    FILE *fisier =fopen(iesire,"a");
    if(fisier==NULL)
    {
        printf("Eroare la deschiderea fisierului\n");
        exit(1);
    }
    for(int i=0;i<n;i++)
    {
        fprintf (fisier, "%s\n" , a[i]);
    }
    fprintf(fisier, "\n");
    fclose(fisier);
}
void scriere_mat_initiala(char **a,int n, const char *iesire)
{
    FILE *fisier =fopen(iesire,"w");
    if(fisier==NULL)
    {
        printf("Eroare la deschiderea fisierului\n");
        exit(1);
    }
    for(int i=0;i<n;i++)
    {
        fprintf (fisier, "%s\n" , a[i]);
    }
    fprintf(fisier, "\n");
    fclose(fisier);
}
int main( int argc, char *argv[])
{
    int n,m,g,t;
    char **a= citire_mat(argv[1], &n, &m, &g, &t);
    scriere_mat_initiala(a, n, argv[2]);
    for (int i=0;i<g;i++)
    {
        act_mat(a,n,m);
        scriere_mat(a,n,argv[2]);
    }
    free_mem(&a, n);
    return 0;
}