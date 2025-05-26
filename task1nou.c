#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
typedef struct 
{
    int linie,coloana;
}ca;

typedef struct Nod
{
    char **mat;
    int h;
    struct Nod *st,*dr;
} Nod;

typedef struct{
    int linie, coloana;
} celula;

typedef struct{
    celula *celule;
    int nr_celule;
}LantHam;

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
int act_mat_nou(char **a,int n, int m, ca **lista)
{
    char **b = aloc_din_mat(n,m);
    *lista = (ca *)malloc(n*m*sizeof(ca));
    if(*lista == NULL) {
        printf("Eroare la alocarea listei\n");
        free_mem(&b, n); 
        exit(1);
    }
    int nr=0;
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
                int vec=numar_vec(a,n,m,i,j);
                char celula =a[i][j];
                char celula_noua;

                if(celula =='X'){
                    if(vec==2 || vec==3){
                    celula_noua='X';
                    } else{
                        celula_noua='+';
                    }
                } else {
                    if(vec==3){
                        celula_noua='X';
                    }else {
                        celula_noua ='+';
                    }
                }
                b[i][j]=celula_noua;
                if(celula != celula_noua)
                {
                    (*lista)[nr].linie =i;
                    (*lista)[nr].coloana =j;
                    nr++;
                }
        }
        b[i][m]= '\0';
    }
    for (int i = 0; i < n; i++) {
        for (int j=0;j<m;j++)
        {
            a[i][j]=b[i][j];
        }
        
    }
    free_mem(&b,n);
    return nr;
}
int cmp_coordonate(const void *a,const void *b)
{
    const ca *x = (const ca *)a;
    const ca *y = (const ca *)b;
    if (x->linie != y->linie)
    return x->linie - y->linie;
    return x->coloana - y->coloana;
}
void afis_stiva(ca **stiva,const int *dim_stiva,int g,const char *output)
{   
    FILE *f =fopen(output, "w");
    if(!f)
    {
        printf("Eroare %s\n", output);
        exit(1);
    }
    for(int i=0;i<g;i++)
    {
    
       
        qsort(stiva[i],dim_stiva[i],sizeof(ca),cmp_coordonate);
        fprintf(f,"%d:",i+1);
        for(int j=0;j<dim_stiva[i];j++)
        {
            fprintf(f,"(%d,%d)",stiva[i][j].linie +1,stiva[i][j].coloana +1);
        }
        fprintf(f,"\n\n");
    }
    fflush(f);
    fclose(f);
}
void scriere_mat(char **a,int n,int m, const char *iesire)
{
    FILE *fisier =fopen(iesire,"a");
    for(int i=0;i<n;i++)
    {
        fprintf (fisier, "%s\n" , a[i]);
    }
    if(fisier == NULL) {
        perror("Eroare la deschiderea fisierului");
        exit(EXIT_FAILURE);
    }
    fprintf(fisier, "\n");
    fflush(fisier); 
    fclose(fisier);
}
void scriere_mat_initiala(char **a,int n,int m, const char *iesire)
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
    fflush(fisier); 
    fclose(fisier);
}

char ** cop_mat(char**a,int n, int m)
{
 char **b=aloc_din_mat(n,m);
 for(int i=0;i<n;i++)
 {
    strcpy(b[i],a[i]);
 }
 return b;
}
char ** regula_noua(char ** a,int n,int m)
{ char **b=aloc_din_mat(n,m);
for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
        int v= numar_vec(a,n,m,i,j);
        if (v==2 || a[i][j]=='X')
        b[i][j]='X';
        else
        b[i][j]='+';
    }
    b[i][m]='\0';
}
return b;
}

Nod * creare_arbor_binar(char **mat_initiala,int n,int m,int k,int h)
{
    if(h>k) return NULL;
    Nod *nod=(Nod*)malloc(sizeof(Nod));
    nod->mat=cop_mat(mat_initiala,n,m);
    if(nod->mat == NULL) {
        free(nod);
        return NULL;
    }
    nod->h=h;
    if(h<k)
    {
       char** mat_st=regula_noua(mat_initiala,n,m); 
       nod->st=creare_arbor_binar(mat_st,n,m,k,h+1); 
       free_mem(&mat_st,n);

        char** mat_dr=cop_mat(mat_initiala,n,m);
        act_mat(mat_dr,n,m);
        nod->dr=creare_arbor_binar(mat_dr,n,m,k,h+1);
        free_mem(&mat_dr,n); 
        
    } else{
        nod->st=NULL;               
        nod->dr=NULL;
    }
    return nod;
}

void parcurgere(Nod *nod,int n, const char *output)
{
    if(!nod) return;
    FILE *f= fopen(output,"a");
    if(!f)
    {
        printf("Eroare\n");
        exit(1);
    }
    for(int i=0;i<n;i++)
       fprintf(f,"%s\n",nod->mat[i]);
       fprintf(f,"\n");
       fclose(f);


       parcurgere(nod->st,n,output);
       parcurgere(nod->dr,n,output);

}

void free_arbore(Nod *nod,int n)
{
    if(!nod) return;
    free_arbore(nod->st,n);
    free_arbore(nod->dr,n);
    free_mem(&nod->mat,n);
    free(nod);
}


celula *gasire_celule_vii(char **mat,int n,int m,int *nr_celule)
{
    celula *celule= (celula*)malloc(n*m*sizeof(celula));
    *nr_celule =0;
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            if(mat[i][j]=='X'){
                celule[*nr_celule].linie=i;
                celule[*nr_celule].coloana=j;
                (*nr_celule)++;
            }
        }
    }
    return celule; 
}

int vecini(celula a,celula b)
{
    int dl=abs(a.linie - b.linie);
    int dc=abs(a.coloana - b.coloana);
    return(dl<=1&&dc<=1 && !(dl ==0 && dc ==0));

}
int ** construire_graf(const celula *celule,int nr_celule)
{
    int **graf = (int**)malloc(nr_celule *sizeof(int*));
    for(int i=0;i<nr_celule;i++)
    {
        graf[i]=(int*)calloc(nr_celule,sizeof(int));

    }
    for(int i=0;i<nr_celule;i++){
        for(int j=i+1;j<nr_celule;j++){
            if(vecini(celule[i],celule[j])){
                graf[i][j]=1;
                graf[j][i]=1;
            }
        }
    }
    return graf;
}


int sortare_celule(const void *a,const void *b)
{
   const celula *x = (const celula*)a;
   const celula *z = (const celula*)b;
   if(x->linie != z->linie){
    return x->linie - z->linie;
   }
   return x->coloana - z->coloana;
}

void dfs(int **graf, int nr_celule,int nod, int *vizitat,int *comp,int comp_nr){
    vizitat[nod]=1;
    comp[nod]=comp_nr;
    for(int i=0;i<nr_celule;i++){
        if(graf[nod][i]&& !vizitat[i]){
            dfs(graf,nr_celule,i,vizitat,comp,comp_nr);
        }
    }
}

void hamiltonian(int **graf,int nr_celule,int *comp,int comp_nr,int nod_curent,int *vizitat,int *lant_curent,int lungime_curenta,int *cel_maibun_lant,int *lmax,celula *celule,int max){
    int nr=0;
    if(max <=0)  return;
    for(int i=0;i<nr_celule;i++){
        if(comp[i]==comp_nr)
        nr++;
    }
    if(lungime_curenta > *lmax || (lungime_curenta==*lmax && lungime_curenta==nr)){
        *lmax=lungime_curenta;
        for(int i=0;i<lungime_curenta;i++){
            cel_maibun_lant[i]=lant_curent[i];
        }
    }
    for(int i=0;i<nr_celule;i++){
        if(comp[i]==comp_nr && !vizitat[i]&& graf[nod_curent][i]){
            int OK =1;
            if(lungime_curenta >0){
                for(int j=0;j<nr_celule;j++){
                    if(comp[j]== comp_nr &&!vizitat[j]&&graf[nod_curent][j]&&j!=i){
                        if(celule[j].linie < celule[i].linie || (celule[j].linie ==celule[i].linie &&celule[j].coloana <celule[i].coloana)){
                            OK=0;
                            break;
                        }
                    }
                }
            }
            if(OK){
                vizitat[i]=1;
                lant_curent[lungime_curenta]=i;
                hamiltonian(graf,nr_celule,comp,comp_nr,i,vizitat,lant_curent,lungime_curenta+1,cel_maibun_lant,lmax,celule,max-1);
                vizitat[i]=0;
            }
        }
    }
}

LantHam gasestelantul(char **mat,int n,int m){
    LantHam rez;
    rez.celule=NULL;
    rez.nr_celule =0;
    int nr_celule;
    celula *celule= gasire_celule_vii(mat,n,m,&nr_celule);
    if(nr_celule == 0)
    {
        free(celule);
        return rez;
    }
    qsort(celule,nr_celule,sizeof(celula),sortare_celule);
    int **graf=construire_graf(celule,nr_celule);
    int *vizitat =(int*)calloc(nr_celule,sizeof(int));
    int *comp=(int*)malloc(nr_celule * sizeof(int));
    int nr_comp=0;
    for(int i=0;i<nr_celule;i++){
        if(!vizitat[i]){
            dfs(graf,nr_celule,i,vizitat,comp,nr_comp);
            nr_comp++;
        }
    }
    int lmax = -1;
    int * cel_maibun_lant = (int*)malloc(nr_celule *sizeof(int));

    for(int c =0 ;c<nr_comp;c++){
        int prim = -1;
        for(int i=0;i<nr_celule;i++){
            if(comp[i]==c){
                prim=i;
                break;
            }
        }
        if(prim != -1){
            int *viz=(int*)calloc(nr_celule,sizeof(int));
            int *lant_curent =(int*)malloc(nr_celule *sizeof(int));
            int *lant_bun =(int*)malloc(nr_celule *sizeof(int));
            int lmax_sec=0;
           for (int start =0;start<nr_celule;start++){
            if(comp[start]!=c) continue;

            
            for(int k=0; k<nr_celule; k++){
                 viz[k] = 0;
                }
            viz[start]=1;
            lant_curent[0]=start;
            int lmax_temp=0;
            hamiltonian(graf,nr_celule,comp,c,start,viz,lant_curent,1,lant_bun,&lmax_temp,celule,1000);
            
            if(lmax_temp > lmax_sec){
                lmax_sec =lmax_temp;
            }
            free(viz);
           }
            int nr_complet =0;
            for(int i=0;i<nr_celule;i++){
                if(comp[i]==c)
                nr_complet++;
            }
            if(lmax_sec==nr_complet &&lmax_sec>lmax){
                lmax=lmax_sec;
                for(int i=0;i<lmax_sec;i++){
                    cel_maibun_lant[i]=lant_bun[i];
                }
            }
            free(viz);
            free(lant_curent);
            free(lant_bun);
        }
    }
    if(lmax>0){
        rez.celule=(celula*)malloc(lmax*sizeof(celula));
        rez.nr_celule=lmax;
        for(int i=0;i<lmax;i++){
            rez.celule[i]=celule[cel_maibun_lant[i]];
        }
    }

    for (int i=0;i<nr_celule;i++){
        free(graf[i]);
    }
    free(graf);
    free(vizitat);
    free(comp);
    free(cel_maibun_lant);
    free(celule);

    return rez;
}


void parcurgere_hamiltonian(Nod *nod,int n,int m,const char *output){
    if(!nod)
    return;

    FILE *f =fopen(output, "a");
    if(!f){
        printf("Eroare\n");
        exit(1);
    }
    LantHam lant = gasestelantul(nod->mat,n,m);

    if(lant.nr_celule == 0){
        fprintf(f,"-1\n");
    }
    else {
        fprintf (f,"%d\n",lant.nr_celule);
        for(int i=0;i<lant.nr_celule;i++){
            fprintf(f,"(%d,%d)", lant.celule[i].linie +1, lant.celule[i].coloana +1);
            if(i<lant.nr_celule -1)
            fprintf(f," ");
        }
        fprintf(f,"\n");
    }
    if(lant.celule)
    free(lant.celule);
    fclose(f);

    parcurgere_hamiltonian(nod->st,n,m,output);
    parcurgere_hamiltonian(nod->dr,n,m,output);
}

//Bonus
void operatie_inversa(ca **stiva, const int *dim_stiva,int g, char **mat_finala,int n,int m,const char *output)
{
    char **mat_curenta = cop_mat(mat_finala,n ,m);
    for(int j=g-1;j>=0;j--){
        for(int i=0;i<dim_stiva[j];i++){
            int linie=stiva[j][i].linie;
            int coloana=stiva[j][i].coloana;

            if(mat_curenta[linie][coloana]=='X'){
                mat_curenta[linie][coloana]='+';
            } else {
                mat_curenta[linie][coloana]='X';
            }
        }
    }

    FILE *f =fopen(output,"w");
    if(!f){
        printf("Eroare\n");
        exit(1);
    }
    for(int i=0;i<n;i++){
        fprintf(f,"%s\n",mat_curenta[i]);
    }
    fprintf(f,"\n");
    fclose(f);

    free_mem(&mat_curenta,n);
}
int main( int argc, char *argv[])
{
    int n,m,g,t;
    char **a= citire_mat(argv[1], &n, &m, &g, &t);
    if(argc < 3) {
        printf("Utilizare: %s intrare iesire [iesire_stiva]\n", argv[0]);
        return 1;
    }
    if (t==1)
    {
        scriere_mat_initiala(a,n,m,argv[2]);
    for (int i=0;i<g;i++)
    {
        act_mat(a,n,m);
        scriere_mat(a,n,m,argv[2]);
    }
    
    free_mem(&a, n);
    }
    
    
   else if (t==2)
   {
    if (argc < 4) {
        fprintf(stderr, "Eroare: Lipseste fisierul de iesire\n");
        exit(EXIT_FAILURE);
    }
    ca **stiva=(ca **)malloc(g*sizeof(ca *));
    int *dim_stiva=(int *)calloc(g,sizeof(int));
    char **mat_initiala = cop_mat(a,n,m);
    for (int i=0;i<g;i++)
    {   dim_stiva[i]=act_mat_nou(a,n,m,&stiva[i]);
       
    }
    afis_stiva(stiva,dim_stiva,g,argv[3]);
    if (argc >=5){
        operatie_inversa(stiva,dim_stiva,g,a,n,m,argv[4]);
    }
    for(int i=0;i<g;i++)
    {
        free(stiva[i]);
        
    }
    free(stiva);
    free(dim_stiva);
    free_mem(&mat_initiala, n);
    free_mem(&a, n);
   }
  

   else if(t==3)
   {
    FILE *f=fopen(argv[2],"w");
    if(f==NULL)
    {
        printf("EROARE\n");
        exit(1);
    }
    fclose(f);

    Nod* root=creare_arbor_binar(a,n,m,g,0);
    parcurgere(root,n,argv[2]);
    free_arbore(root,n);
    free_mem(&a, n);
   }

   else if (t==4)
   {
    FILE *f=fopen(argv[2],"w");
    if(f==NULL){
        printf("Eroare\n");
        exit(1);
    }
    fclose(f);

    Nod* root = creare_arbor_binar(a,n,m,g,0);
    parcurgere_hamiltonian(root,n,m,argv[2]);
    free_arbore(root,n);
    free_mem(&a,n);
   }
    return 0;
}
