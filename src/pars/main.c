#include <stdio.h>

int get_state(int prev_state, int pos_state)
{
    int states[5][5] = {
        {0, 1, 2, 3, 4}, //inicio
        {0, 1, 1, 1, 1}, //error
        {0, 1, 1, 3, 1}, //simbolos 2 
        {0, 1, 1, 3, 4}, //digitos 3 
        {0, 1, 1, 3, 4}, //espacio 4
    };
    return (states[prev_state][pos_state]);
}

int automata(int prev_state, char a)
{
    int pos = 0;
    int new_state = 0;

    if (a == '+' || a == '-')
        pos = 2;
    else if (a >= '0' && a <= '9')
        pos = 3;
    else if (a >= ' ')
        pos = 4;
    else
        pos = 1;
    new_state = get_state(prev_state, pos);
    printf("prev %d char %c new %d\n", prev_state, a, new_state);
    return (new_state);
}

void evaluate(char *str)
{
    int i = 0;
    int state = 0;

    while (str[i])
    {
        state = automata(state, str[i]);
        i++;
        if (state == 1)
            return;
        
    }
    
}

int main(void)
{
    evaluate("-42 312 9843 0 1");
}
