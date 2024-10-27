#include <stdio.h>
#include <stdlib.h>

void linearInterpolation(const double *inputArray, int inputLength, double *outputArray, int outputLength) {
    double scaleFactor = (double)(inputLength - 1) / (outputLength - 1);

    for (int i = 0; i < outputLength; ++i) {
        double index = i * scaleFactor;
        int lowerIndex = (int)index;
        int upperIndex = lowerIndex + 1;

        if (upperIndex >= inputLength) {
            upperIndex = inputLength - 1;
        }

        double fraction = index - lowerIndex;

        outputArray[i] = (1.0 - fraction) * inputArray[lowerIndex] + fraction * inputArray[upperIndex];
    }
}

int main() {
    // Example usage:
    int originalLength = 5;
    double originalArray[] = {1.0, 2.0, 4.0, 8.0, 16.0};

    int targetLength = 3;
    double interpolatedArray[targetLength];

    linearInterpolation(originalArray, originalLength, interpolatedArray, targetLength);

    // Display the result
    printf("Original Array: ");
    for (int i = 0; i < originalLength; ++i) {
        printf("%.2f ", originalArray[i]);
    }

    printf("\nInterpolated Array: ");
    for (int i = 0; i < targetLength; ++i) {
        printf("%.2f ", interpolatedArray[i]);
    }

    return 0;
}
