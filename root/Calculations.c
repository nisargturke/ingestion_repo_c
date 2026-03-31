/* Big Arithmetic Calculation:*/

#include <stdio.h>
#include <string.h>

#define MAX 1000 // Maximum number of digits

// Function to perform addition of two large numbers
void bigAddition(char num1[], char num2[], char result[]) 
{
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    int carry = 0, sum, i, j, k = 0;

    // Reverse the numbers for easier addition from least significant digit
    char temp1[MAX], temp2[MAX];
    for (i = 0; i < len1; i++) temp1[i] = num1[len1 - i - 1];
    for (i = 0; i < len2; i++) temp2[i] = num2[len2 - i - 1];
    temp1[len1] = '\0';
    temp2[len2] = '\0';

    // Perform addition digit by digit
    for (i = 0, j = 0; i < len1 || j < len2 || carry; i++, j++, k++) 
	{
        int digit1 = (i < len1) ? temp1[i] - '0' : 0;
        int digit2 = (j < len2) ? temp2[j] - '0' : 0;

        sum = digit1 + digit2 + carry;
        result[k] = (sum % 10) + '0'; // Store the current digit
        carry = sum / 10;            // Calculate carry
    }

    result[k] = '\0';

    // Reverse the result to get the final answer
    for (i = 0; i < k / 2; i++) 
	{
        char temp = result[i];
        result[i] = result[k - i - 1];
        result[k - i - 1] = temp;
    }
}

int main() 
{
    char num1[MAX], num2[MAX], result[MAX];

    printf("Enter the first large number: ");
    scanf("%s", num1);

    printf("Enter the second large number: ");
    scanf("%s", num2);

    bigAddition(num1, num2, result);

    printf("The sum of the two large numbers is: %s\n", result);

    return 0;
}