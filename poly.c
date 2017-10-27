#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <errno.h>

typedef struct Monomio Monomio;

typedef struct
{
    Monomio *primeiro;
    Monomio *ultimo;
    
    long int constante;
    
    int grau;
    
    int termos;
    
}Polinomio;

struct Monomio
{   
    unsigned short int expoente;
    
    long int constante;
    
    Monomio *prox;
    
    Monomio *ant;
    
};

Polinomio* PolinomioCreate( void )
{
    
    Polinomio* polinomio = calloc( 1, sizeof(Polinomio) );
    
    if( !polinomio )
    {
        return NULL;
    }
    
    return polinomio;
    
}

void PolinomioInsert( Polinomio *polinomio , Monomio *monomio )
{   
    
    Monomio *aux;
    
    if( !polinomio->primeiro )
    {
        polinomio->primeiro = monomio;
        
        polinomio->ultimo = monomio;
        
        polinomio->grau = monomio->expoente;
        
        polinomio->termos++;
        
        return;
        
    }
    
    aux = polinomio->primeiro;
    
    while( aux )
    {
       
        
        if( monomio->expoente > aux->expoente )
        {
            
            monomio->prox = aux;
            
            monomio->ant = aux->ant;
            
            aux->ant = monomio;
            
            if( monomio->ant )
            {
                monomio->ant->prox = monomio;
            }
            
            if( aux == polinomio->primeiro )
            {
                polinomio->grau = monomio->expoente;
                polinomio->primeiro = monomio;
            }
            
            
            polinomio->termos++;
            
            return;
        }
        
        
        if( monomio->expoente == aux->expoente )
        {
            
            
            aux->constante += monomio->constante;
            
            free( monomio );
            
            if( !aux->constante )
            {
                
                if( polinomio->primeiro == aux )
                {
                    polinomio->primeiro = aux->prox;
                    
                    if(aux->prox )
                    {
                        polinomio->grau = aux->prox->expoente;
                    }
                    else
                    {
                        polinomio->grau = 0;
                    }
                }
                else if( polinomio->ultimo == aux )
                {
                    
                    polinomio->ultimo = aux->ant;
                
                }
                else
                {
                    aux->ant->prox = aux->prox;
                
                    aux->prox->ant = aux->ant;
                }
                
                polinomio->termos--;
                
                free( aux );
                
               
                
            }
            
            
            
            
            return;
            
        }
        
        aux = aux->prox;
        
        
    }
    
    
    polinomio->ultimo->prox = monomio;
    
    polinomio->ultimo = monomio;
    
    polinomio->termos++;
    
}

void PolinomioDestroy( Polinomio* polinomio)
{
    Monomio *monomio = polinomio->primeiro;
    Monomio *aux;
    
    while( monomio  )
    {
        aux = monomio;
        
        monomio = monomio->prox;
        
        free(aux);
        
    }
   
   
    free(polinomio);
    
}

void PolinomioPrint( Polinomio* polinomio )
{
    
    Monomio *monomio = polinomio->primeiro;
    
    while( monomio )
    {
        if( monomio->constante != 1 && monomio->constante != -1 )
        {
            
            if( monomio-> constante < 0)
            {
                printf("%dx^%d ",monomio->constante,monomio->expoente);
            }
            else
            {
                printf("+%dx^%d ",monomio->constante, monomio->expoente );
            }
        }
        else
        {
            printf("%cx^%d " , monomio->constante > 0 ? '+':'-',monomio->expoente);
        }
        
        monomio = monomio->prox;
    }
    
    if( polinomio->constante < 0)
    {
        
        printf("%d\n ",polinomio->constante);
    
    }
    else if( polinomio->constante > 0 )
    {
        
        printf("+%d\n ",polinomio->constante);
        
    }
    else
    {
        putchar('\n');
    }
    
}

double PolinomioNumericValue( Polinomio* polinomio, double x )
{
    double result = 0;
    
    Monomio *monomio = polinomio->primeiro;
    
    while( monomio )
    {
        result += monomio->constante * ( pow( x, monomio->expoente) );
       
        monomio = monomio->prox; 
    }
    
    return result + polinomio->constante;
    

}

bool PolinomioError( Polinomio* polinomio , Monomio *monomio )
{
    free( monomio );
    
    PolinomioDestroy( polinomio );
    
    return false;
}
  
bool PolinomioParse(char *input, Polinomio *polinomio )
{
    
  
    
    char *p = NULL;
    char *p2 = NULL;
    
    long int constante;
    
    int sinal = 1;
    
    Monomio *monomio;
 
    p = input ;
    
    while( *p )
    {
        
        monomio = calloc( 1 , sizeof( Monomio ) );
        
        if( !monomio)
        {
            return PolinomioError( polinomio , monomio );
        }
        
        while( *p == ' ')
        {
            p++;
        }
        
        if( *p == '-' )
        {
            sinal = -1 ;
            p++;
        }
        else if ( *p == '+' )
        {
            sinal = 1;
            p++;
        }
        else if( p != input )
        {
            return PolinomioError( polinomio , monomio );
        }
        errno = 0;
        
        constante = strtol( p,&p2,10);
        
        if( errno == ERANGE )
        {
           
            return PolinomioError( polinomio, monomio );
            
        }
        
        p = p2;
        
        while( *p == ' ' )// igonara espaços
        {
            p++;
        }
        
        if( *p == 'x' || *p == 'X' )
        {
            p++;
            
            if( !constante )
            {
                constante = sinal;
            }
            else
            {
                constante *= sinal;
            }
        
            monomio->constante = constante;
           
            while( *p == ' ' )
            {
                p++;
            }
            
            if( *p == '^' )
            {   
                
                p++;
                
                errno = 0;
                
                monomio->expoente = ( unsigned short int) strtol( p , &p2 , 10 );
        
                if( errno == ERANGE || monomio->expoente > 50 )
                {  
                    return PolinomioError( polinomio, monomio );
                }
                
                p = p2;
                
            }
            else 
            {
                monomio->expoente = 1;
            }
            
            PolinomioInsert( polinomio , monomio );
            
        }
        else if( *p == '-' || *p == '+' || *p == '\0' )
        {
            polinomio->constante += sinal*constante;
        }
        else
        {
            return PolinomioError( polinomio , monomio ); 
        }
    
    
    }

    
    return true;
   
}

int main( int argc , char **argv )
{
    if( argc < 3) 
    {
        printf("\n\tUso: %s \"<expression>\" <num>\n\tExemplo: %s \"x^2-2x+1\" 2\n",*argv,*argv);
        return EXIT_FAILURE;
    }
    
    
    Polinomio* poly = PolinomioCreate( );
    
    if( poly )
    {
        
        if( !PolinomioParse( argv[1] ,poly ) )
        {
            puts("erro ao fazer parse do polinomio");
            exit(EXIT_FAILURE);
        }
        
    }
    
    PolinomioPrint( poly ); 
    
    printf("Valor numerico para P( %s ) = %.0lf",argv[2],PolinomioNumericValue( poly, atof( argv[2] ) ) );
    
    PolinomioDestroy( poly );
    
    return EXIT_SUCCESS;
    
    
}