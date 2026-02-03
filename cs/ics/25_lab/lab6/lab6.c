#include <stdio.h>
#include <stdlib.h>

/* lab1 */
int cond(int cond, int val1, int val2){
    return cond ? val1 : val2;
}

/* lab2 */
int Hofstadter_rec(int n){
    if(n <= 0){
        return 0;
    } else if(n == 1 || n == 2){
        return 1;
    } else {
        return Hofstadter_rec(n - Hofstadter_rec(n - 1)) + Hofstadter_rec(n - Hofstadter_rec(n - 2));
    }
}

int Hofstadter_iter(int n){
    if(n <= 0){
        return 0;
    }else if(n == 1 || n == 2){
        return 1;
    } else {
        int *H = (int *)malloc((n + 1) * sizeof(int));
        H[0] = 0;
        H[1] = 1;
        H[2] = 1;
        for(int i = 3; i <= n; i++){
            H[i] = H[i - H[i - 1]] + H[i - H[i - 2]];
        }
        int result = H[n];
        free(H);
        return result;
    }
}

/* lab3 */
unsigned int mod7(){
    char *x = (char *)malloc(100 * sizeof(char));
    unsigned int result = 0;
    unsigned int x_decimal = 0;
    gets(x);  // Read binary input as string
    for(int i = 0; x[i] != 'y'; i++){
        x_decimal = (x_decimal * 2 + x[i] - '0');
    }
    free(x);
    return x_decimal % 7;
}

/* lab4 */
int calculate_routes(int i , int j){
    if(i == 0 || j == 0)
        return 1;
    return calculate_routes(i - 1, j) + calculate_routes(i, j - 1);
}
int calculate_recommendation(int i, int j){
    if(i < 0 || j < 0){
        printf("Invalid input\n");
        return -1;
    }
    int steps = i + j;
    int routes = calculate_routes(i, j);
    printf("Number of steps: %d\n", steps);
    printf("Number of routes: %d\n", routes);
    return routes * 5 - steps;
}

/* lab5 */
void buffer_attack(){
    char buffer[10];
    printf("Enter some text: ");
    gets(buffer);  // Unsafe function, used here for demonstration purposes only
    printf("You entered: %s\n", buffer);
}