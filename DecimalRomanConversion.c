#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function prototypes
int romanToDecimal(const char *roman);
int isValidRoman(const char *roman);
void decimalToWords(long long num, char *words);
void processFile(const char *inputFile, const char *outputFile);

int main() {
    // Call the function to process input and output files
    processFile("input.txt", "output.txt");
    // Inform the user that the processing is complete
    printf("Processing completed. Check 'output.txt' for results.\n");
    return 0;
}

// Function to convert a Roman numeral to a decimal value
int romanToDecimal(const char *roman) {
    // Map Roman numeral characters to their respective values
    int values[256] = {0};
    values['I'] = 1;
    values['V'] = 5;
    values['X'] = 10;
    values['L'] = 50;
    values['C'] = 100;
    values['D'] = 500;
    values['M'] = 1000;

    int total = 0;  // Stores the final decimal value
    int prevValue = 0;  // Tracks the previous numeral's value

    // Process the Roman numeral string from right to left
    for (int i = strlen(roman) - 1; i >= 0; i--) {
        int currentValue = values[(unsigned char)roman[i]];
        // Add or subtract based on Roman numeral rules
        if (currentValue >= prevValue) {
            total += currentValue;
        } else {
            total -= currentValue;
        }
        prevValue = currentValue;  // Update the previous value
    }

    return total;
}

// Function to validate the formatting of a Roman numeral
int isValidRoman(const char *roman) {
    // Map Roman numeral characters to their respective values
    int values[256] = {0};
    values['I'] = 1;
    values['V'] = 5;
    values['X'] = 10;
    values['L'] = 50;
    values['C'] = 100;
    values['D'] = 500;
    values['M'] = 1000;

    int maxRepeat = 3;  // Maximum repetitions allowed for I, X, C, and M
    int repeatCount = 1;  // Count of consecutive repeated characters

    for (size_t i = 0; i < strlen(roman); i++) {
        // Check for invalid characters
        if (values[(unsigned char)roman[i]] == 0) {
            return 0;  // Invalid character found
        }

        // Check for repetitions
        if (i > 0 && roman[i] == roman[i - 1]) {
            repeatCount++;
            if (repeatCount > maxRepeat) {
                return 0;  // Invalid repetition
            }
        } else {
            repeatCount = 1;  // Reset repetition count
        }

        // Check subtraction rules
        if (i > 0 && values[(unsigned char)roman[i]] > values[(unsigned char)roman[i - 1]]) {
            // Ensure only valid subtractive combinations are used
            if (roman[i - 1] != 'I' && roman[i - 1] != 'X' && roman[i - 1] != 'C') {
                return 0;  // Invalid subtraction
            }
            if ((roman[i - 1] == 'I' && roman[i] != 'V' && roman[i] != 'X') ||
                (roman[i - 1] == 'X' && roman[i] != 'L' && roman[i] != 'C') ||
                (roman[i - 1] == 'C' && roman[i] != 'D' && roman[i] != 'M')) {
                return 0;  // Invalid subtraction combination
            }
        }
    }

    return 1;  // Valid Roman numeral
}

// Function to convert a decimal number to its word representation
void decimalToWords(long long num, char *words) {
    // Arrays to map numbers to words
    const char *ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    const char *teens[] = {"", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    const char *tens[] = {"", "Ten", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    const char *thousands[] = {"", "Thousand", "Million", "Billion", "Trillion"};

    char temp[1000] = "";  // Temporary buffer for building the word representation

    if (num == 0) {
        strcpy(words, "Zero");
        return;
    }

    int isNegative = 0;  // Flag for negative numbers
    if (num < 0) {
        isNegative = 1;
        num = -num;  // Convert to positive for processing
    }

    int groupIndex = 0;  // Tracks the group (e.g., Thousand, Million)
    while (num > 0) {
        int group = num % 1000;  // Extract the last three digits
        if (group > 0) {
            char groupWords[256] = "";  // Buffer for the current group
            if (group / 100 > 0) {
                strcat(groupWords, ones[group / 100]);
                strcat(groupWords, " Hundred ");
            }
            if (group % 100 >= 11 && group % 100 <= 19) {
                strcat(groupWords, teens[group % 10]);  // Handle teens (11-19)
            } else {
                strcat(groupWords, tens[(group % 100) / 10]);  // Handle tens (20, 30, ...)
                if (group % 10 > 0) {
                    strcat(groupWords, " ");
                    strcat(groupWords, ones[group % 10]);  // Handle ones (1-9)
                }
            }
            if (groupIndex > 0) {
                strcat(groupWords, " ");
                strcat(groupWords, thousands[groupIndex]);  // Add group name (Thousand, Million, etc.)
            }
            strcat(groupWords, " ");
            memmove(temp + strlen(groupWords), temp, strlen(temp) + 1);  // Shift existing text
            strncpy(temp, groupWords, strlen(groupWords));  // Prepend current group's words
        }
        num /= 1000;  // Remove the last three digits
        groupIndex++;  // Move to the next group
    }

    if (isNegative) {
        char negativePrefix[1000] = "Negative ";
        strcat(negativePrefix, temp);
        strcpy(temp, negativePrefix);  // Add the "Negative" prefix
    }

    strcpy(words, temp);  // Copy the result to the output buffer
}

// Function to process the input and output files
void processFile(const char *inputFile, const char *outputFile) {
    FILE *input = fopen(inputFile, "r");
    FILE *output = fopen(outputFile, "w");

    if (!input || !output) {
        printf("Error opening files.\n");
        return;
    }

    char line[256];  // Buffer to store each line of input
    while (fgets(line, sizeof(line), input)) {
        char roman1[50], roman2[50], operation;
        // Parse the line for two Roman numerals and an operation
        if (sscanf(line, "%s %c %s", roman1, &operation, roman2) != 3) {
            fprintf(output, "Invalid input: %s", line);
            continue;
        }

        // Validate the Roman numerals
        if (!isValidRoman(roman1) || !isValidRoman(roman2)) {
            fprintf(output, "Invalid Roman numerals: %s", line);
            continue;
        }

        int num1 = romanToDecimal(roman1);
        int num2 = romanToDecimal(roman2);
        long long result;

        // Perform the specified operation
        switch (operation) {
            case '+':
                result = num1 + num2;
                break;
            case '-':
                result = num1 - num2;
                break;
            case '*':
                result = (long long)num1 * num2;
                break;
            case '/':
                if (num2 == 0) {
                    fprintf(output, "Division by zero: %s", line);
                    continue;
                }
                result = num1 / num2;
                break;
            default:
                fprintf(output, "Invalid operation: %s", line);
                continue;
        }

        // Convert the result to words
        char words[1000] = "";
        decimalToWords(result, words);

        // Write the result to the output file
        fprintf(output, "%s %c %s = %lld -> %s\n", roman1, operation, roman2, result, words);
    }

    fclose(input);  // Close the input file
    fclose(output);  // Close the output file
}
