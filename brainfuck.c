/*this is an small interpreter of brainfuck language£®¬“»Î£©*/

#include <stdio.h>
#include<stdlib.h> 
#include<string.h>
int  p, r, q;
char a[5000], b, o;

/*process the input string*/
void interpret(char *c)
{
    char *d;
    r++;
    while( *c ) {
        switch(o=1,*c++) { // the seven opreators:
            case '<': p--;        break;//move the pointer
			case '>': p++;        break;//move the pointer
            case '+': a[p]++;     break;//increase the digit
            case '-': a[p]--;     break;//decrease the digit
            case '.': putchar(a[p]); fflush(stdout); break;//out put the digit
            case ',': a[p]=getchar();fflush(stdout); break;//input the digit
            case '[': //process a roop
                for( b=1,d=c; b && *c; c++ )
					b+=*c=='[', b-=*c==']';
                if(!b) {
                    c[-1]=0;
                    while( a[p] )
                    interpret(d);
                    c[-1]=']';
                    break;
                }
            case ']':
                puts("UNBALANCED BRACKETS"), exit(0);
            case '#':
  if(q>2)
                printf("- - - - - - - - - -/n%*s/n",*a,a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8],a[9],3*p+2,"^");
                break;
            default: o=0;
        }
 if( p<0 || p>100)
            puts("RANGE ERROR"), exit(0);
    }
    r--;
}


