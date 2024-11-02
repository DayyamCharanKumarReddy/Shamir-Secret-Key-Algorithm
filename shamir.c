#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_KEYS 10

// Structure to hold each (x, y) point
typedef struct {
    int x;
    long y;
} Point;

// Convert a number from a given base to base 10
long convert_to_base10(const char *value, int base) {
    long result = 0;
    int length = strlen(value);
    for (int i = 0; i < length; i++) {
        char digit = value[i];
        int num;

        if (digit >= '0' && digit <= '9') {
            num = digit - '0';
        } else if (digit >= 'A' && digit <= 'F') { // for hexadecimal bases
            num = 10 + (digit - 'A');
        } else {
            return -1; // invalid character for the base
        }

        if (num >= base) {
            return -1; // invalid digit for the base
        }

        result = result * base + num;
    }
    return result;
}

// Parse the JSON input and populate the points
int parse_input(const char *filename, Point points[], int *k) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Unable to open file %s\n", filename);
        return 0;
    }

    char line[256];
    int index = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "\"k\"")) {
            sscanf(line, " \"k\": %d,", k);
        }
        else if (strstr(line, "base")) {
            int base, x;
            char value[64];

            sscanf(line, " \"%d\": { \"base\": \"%d\", \"value\": \"%[^\"]\" }", &x, &base, value);
            points[index].x = x;
            points[index].y = convert_to_base10(value, base);
            index++;
        }
    }

    fclose(file);
    return index; // Number of points parsed
}

// Calculate the secret using Lagrange interpolation
long calculate_secret(Point points[], int k) {
    long secret = 0;

    for (int i = 0; i < k; i++) {
        double term = points[i].y;

        for (int j = 0; j < k; j++) {
            if (i != j) {
                term *= (0 - points[j].x) / (double)(points[i].x - points[j].x);
            }
        }

        secret += term;
    }

    return (long)round(secret); // Return the constant term (secret)
}

int main() {
    Point points[MAX_KEYS];
    int k;

    // Parse input files (testcase1.json and testcase2.json)
    int num_points1 = parse_input("testcase1.json", points, &k);
    if (num_points1 >= k) {
        long secret1 = calculate_secret(points, k);
        printf("Secret for Test Case 1: %ld\n", secret1);
    } else {
        printf("Not enough points for Test Case 1\n");
    }

    int num_points2 = parse_input("testcase2.json", points, &k);
    if (num_points2 >= k) {
        long secret2 = calculate_secret(points, k);
        printf("Secret for Test Case 2: %ld\n", secret2);
    } else {
        printf("Not enough points for Test Case 2\n");
    }

    return 0;
}
