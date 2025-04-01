#include <stdio.h>

#define DIGIT_HEIGHT 64
#define DIGIT_WIDTH  32
#define DIGIT_THICKNESS 4

void printDigit(int digit[DIGIT_HEIGHT][DIGIT_WIDTH]) {
    for (int i = 0; i < DIGIT_HEIGHT; i++) {
        for (int j = 0; j < DIGIT_WIDTH; j++) {
            printf("%d", digit[i][j]);
			printf("%d", digit[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void drawHorizontalSegment(int digit[DIGIT_HEIGHT][DIGIT_WIDTH], int y) {
    for (int i = 0; i < DIGIT_THICKNESS; i++) {
        for (int j = 2; j < DIGIT_WIDTH - 2; j++) {
            digit[y + i][j] = 1;
        }
    }
}

void drawVerticalSegment(int digit[DIGIT_HEIGHT][DIGIT_WIDTH], int x, int yStart, int yEnd) {
    for (int i = 0; i < DIGIT_THICKNESS; i++) {
        for (int j = yStart; j < yEnd; j++) {
            digit[j][x + i] = 1;
        }
    }
}

void generateDigit(int digit[DIGIT_HEIGHT][DIGIT_WIDTH], int num) {
    // Clear the array
    for (int i = 0; i < DIGIT_HEIGHT; i++)
        for (int j = 0; j < DIGIT_WIDTH; j++)
            digit[i][j] = 0;

    // Define which segments should be filled for each digit
    int segments[10][7] = {
        {1, 1, 1, 0, 1, 1, 1}, // 0
        {0, 1, 0, 0, 1, 0, 0}, // 1
        {1, 1, 0, 1, 0, 1, 1}, // 2
        {1, 1, 0, 1, 1, 0, 1}, // 3
        {0, 1, 1, 1, 1, 0, 0}, // 4
        {1, 0, 1, 1, 1, 0, 1}, // 5
        {1, 0, 1, 1, 1, 1, 1}, // 6
        {1, 1, 0, 0, 1, 0, 0}, // 7
        {1, 1, 1, 1, 1, 1, 1}, // 8
        {1, 1, 1, 1, 1, 0, 1}  // 9
    };

    int mid = DIGIT_HEIGHT / 2;
    int bottom = DIGIT_HEIGHT - DIGIT_THICKNESS - 2;

    if (segments[num][0]) drawHorizontalSegment(digit, 2); // Top
    if (segments[num][1]) drawVerticalSegment(digit, DIGIT_WIDTH - DIGIT_THICKNESS - 2, 2, mid); // Upper Right
    if (segments[num][2]) drawVerticalSegment(digit, 2, 2, mid); // Upper Left
    if (segments[num][3]) drawHorizontalSegment(digit, mid - DIGIT_THICKNESS / 2); // Middle
    if (segments[num][4]) drawVerticalSegment(digit, DIGIT_WIDTH - DIGIT_THICKNESS - 2, mid, bottom); // Lower Right
    if (segments[num][5]) drawVerticalSegment(digit, 2, mid, bottom); // Lower Left
    if (segments[num][6]) drawHorizontalSegment(digit, bottom); // Bottom
}

int main() {
    int digits[10][DIGIT_HEIGHT][DIGIT_WIDTH];
    for (int i = 0; i < 10; i++) {
        generateDigit(digits[i], i);
        printf("Digit %d:\n", i);
        printDigit(digits[i]);
    }
    return 0;
}
