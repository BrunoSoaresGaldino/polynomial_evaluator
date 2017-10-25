#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>


typedef struct Monomio Monomio;

typedef struct
{
    Monomio *primeiro;
    Monomio *ultimo;
    
    int constante;
    
    int grau;
    
    int termos;
    
}Polinomio;

struct Monomio
{   
    unsigned int expoente;
    
    int constante;
    
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

void PolinomioAdd( Polinomio *polinomio , Monomio *monomio )
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
  
bool ParsePolinomio(char *input, Polinomio *polinomio )
{
    
  
    
    char *p = NULL;
    
    char sinal = '+';
    
    char aux[101] = "";
    
    int i;
    
    Monomio *monomio;
 
    p = input ;
    
    while( *p )
    {
        
        monomio = calloc( 1 , sizeof( Monomio ) );
        
        if( !monomio)
        {
            return PolinomioError( polinomio , monomio );
        }
        
  
        i = 0;
        
        while( *p == ' ')
        {
            p++;
        }
        
        
        if( *p == '-')
        {
            sinal = '-';
            p++;
        }
        else if( *p == '+')
        {
            sinal = '+';
            p++;
        }
        
        while( *p == ' ')
        {
            p++;
        }
        
        while( isdigit(*p) ) 
        {
            if( i == 100 )
            {
                
                fputs("Constante muito grande",stderr);
                
                return PolinomioError( polinomio , monomio );
                
            }
            
            aux[i++] = *p;
            
            p++;
            
        }
        
        aux[i] = '\0';
        
        if( *p == '+' || *p == '-' || *p == '\0')
        {
            
        
            if( sinal == '-' )
            {
                
                polinomio->constante  -= atoi( aux );
                
                continue;
                
            }
            else if( sinal == '+' || isdigit(sinal) )
            {
                polinomio->constante  += atoi( aux );
                
                continue;
            }
            
        
        }
        else if( *p == 'x' || *p == 'X' )
        {
           
           
            if( i == 0 )// caso não haja uma constante acompanhando a váriavel
            {
               
                if( sinal == '-' )
                {
                
                    monomio->constante = -1;
                
                }
                else
                {

                    monomio->constante = 1;
                }   
                
            }// i != 0
            else
            {
                if( sinal == '-' )
                {
                    monomio->constante = -atoi( aux );
                }
                else
                {
                    monomio->constante = atoi( aux );
                }
            }
               
               
            p++;// avanca depois de encontrado x
            
            i = 0;
        
            if( *p == '^' )
            {
                p++;
                
                while( *p == ' ')//ignora espaços
                {
                    p++;
                }
                
                while( isdigit( *p ) )
                {
                    
                    if( i == 4 )
                    {
                        fputs("expoente muito grande\n",stderr);
                        
                        return PolinomioError( polinomio , monomio );
                    
                    }
                    
                    aux[i++] = *p;
                    
                    p++;
                    
                }
                
                aux[i] = '\0';
                
                
                if( !i || ( *p != '-' && * p != '+' && *p != '\0' && *p != ' ') )
                {
                    return PolinomioError( polinomio , monomio );
                }
                
                monomio->expoente = atoi( aux );
                
                if( monomio-> expoente > 50 )
                {
                    fputs("O expoente nao pode ser maior que 50\n",stderr);
                    
                    return PolinomioError( polinomio , monomio );
                }
               
                
            }// não achou ^
            else
            {
                monomio->expoente = 1;   
            }
        
        
        }  
        else
        {
            
            return PolinomioError( polinomio , monomio );
        }
    
        
        PolinomioAdd( polinomio , monomio );
    
    
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
        
        if( !ParsePolinomio( argv[1] ,poly ) )
        {
            puts("erro ao fazer parse do polinomio");
            exit(EXIT_FAILURE);
        }
        
    }
    
    PolinomioPrint( poly ); 
    
    printf("Valor numerico para P( %s ) = %.0lf",argv[2],PolinomioNumericValue( poly, atof( argv[2] ) ) );
    
    return EXIT_SUCCESS;
    
    
}