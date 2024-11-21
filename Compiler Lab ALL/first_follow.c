#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 10

int num_productions;
char productions[MAX][MAX], first_sets[MAX][MAX], follow_sets[MAX][MAX];
int computed_first[MAX], computed_follow[MAX];

void compute_first(char symbol);
void compute_follow(char symbol);
void add_to_set(char *set, char symbol);
int is_non_terminal(char symbol);
int is_in_set(const char *set, char symbol);

int main()
{
    printf("Enter number of productions: ");
    scanf("%d", &num_productions);

    printf("Enter the productions (E.g., E=E+T or E->E+T):\n");
    for (int i = 0; i < num_productions; i++)
    {
        scanf("%s", productions[i]);
        computed_first[i] = 0;    // Mark FIRST not computed for each non-terminal
        computed_follow[i] = 0;   // Mark FOLLOW not computed for each non-terminal
        first_sets[i][0] = '\0';  // Initialize empty FIRST set
        follow_sets[i][0] = '\0'; // Initialize empty FOLLOW set
    }

    // Calculate FIRST sets for all non-terminals
    for (int i = 0; i < num_productions; i++)
    {
        compute_first(productions[i][0]);
    }

    // Add $ (EOF marker) to the FOLLOW set of the start symbol
    add_to_set(follow_sets[0], '$');

    // Calculate FOLLOW sets for all non-terminals
    for (int i = 0; i < num_productions; i++)
    {
        compute_follow(productions[i][0]);
    }

    // Display FIRST and FOLLOW sets
    printf("\nFIRST and FOLLOW sets:\n");
    for (int i = 0; i < num_productions; i++)
    {
        printf("FIRST(%c) = { %s }\n", productions[i][0], first_sets[i]);
        printf("FOLLOW(%c) = { %s }\n", productions[i][0], follow_sets[i]);
    }

    return 0;
}

// Compute FIRST set for a non-terminal
void compute_first(char symbol)
{
    if (computed_first[symbol - 'A'])
        return; // Already computed

    computed_first[symbol - 'A'] = 1; // Mark as computed
    for (int i = 0; i < num_productions; i++)
    {
        if (productions[i][0] == symbol)
        {
            for (int j = 2; productions[i][j] != '\0'; j++)
            {
                char current = productions[i][j];
                if (!is_non_terminal(current))
                {
                    // Add terminal to FIRST set
                    add_to_set(first_sets[symbol - 'A'], current);
                    break;
                }
                else
                {
                    // Add FIRST of the non-terminal to current FIRST set
                    compute_first(current);
                    for (int k = 0; first_sets[current - 'A'][k] != '\0'; k++)
                    {
                        if (first_sets[current - 'A'][k] != '#') // '#' represents epsilon
                            add_to_set(first_sets[symbol - 'A'], first_sets[current - 'A'][k]);
                    }
                    if (!is_in_set(first_sets[current - 'A'], '#'))
                        break; // Stop if epsilon is not in FIRST
                }
            }
        }
    }
}

// Compute FOLLOW set for a non-terminal
void compute_follow(char symbol)
{
    if (computed_follow[symbol - 'A'])
        return; // Already computed

    computed_follow[symbol - 'A'] = 1; // Mark as computed
    for (int i = 0; i < num_productions; i++)
    {
        for (int j = 2; productions[i][j] != '\0'; j++)
        {
            if (productions[i][j] == symbol)
            {
                // Check FOLLOW of productions[i][0]
                if (productions[i][j + 1] == '\0')
                {
                    compute_follow(productions[i][0]);
                    for (int k = 0; follow_sets[productions[i][0] - 'A'][k] != '\0'; k++)
                    {
                        add_to_set(follow_sets[symbol - 'A'], follow_sets[productions[i][0] - 'A'][k]);
                    }
                }
                else
                {
                    char next = productions[i][j + 1];
                    if (!is_non_terminal(next))
                    {
                        add_to_set(follow_sets[symbol - 'A'], next);
                    }
                    else
                    {
                        for (int k = 0; first_sets[next - 'A'][k] != '\0'; k++)
                        {
                            if (first_sets[next - 'A'][k] != '#')
                                add_to_set(follow_sets[symbol - 'A'], first_sets[next - 'A'][k]);
                        }
                        if (is_in_set(first_sets[next - 'A'], '#'))
                        {
                            compute_follow(productions[i][0]);
                            for (int k = 0; follow_sets[productions[i][0] - 'A'][k] != '\0'; k++)
                            {
                                add_to_set(follow_sets[symbol - 'A'], follow_sets[productions[i][0] - 'A'][k]);
                            }
                        }
                    }
                }
            }
        }
    }
}

// Add a symbol to a set if it is not already present
void add_to_set(char *set, char symbol)
{
    if (!is_in_set(set, symbol))
    {
        int len = strlen(set);
        set[len] = symbol;
        set[len + 1] = '\0';
    }
}

// Check if a symbol is in a set
int is_in_set(const char *set, char symbol)
{
    for (int i = 0; set[i] != '\0'; i++)
    {
        if (set[i] == symbol)
            return 1;
    }
    return 0;
}

// Check if a symbol is a non-terminal
int is_non_terminal(char symbol)
{
    return isupper(symbol);
}
