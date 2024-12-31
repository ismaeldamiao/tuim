long long __aeabi_lasr(long long n, int shift) {
    if (shift < 0 || shift >= 64) {
        // Handle invalid shift value
        // You can either return an error or abort the program
        // For this example, we'll just return 0
        return 0;
    }

    unsigned int high = (unsigned int)(n >> 32);
    unsigned int low = (unsigned int)n;

    if (n < 0) {
        high = ~high + 1;
        low = ~low + 1;
    }

    if (shift < 32) {
        low = (low >> shift) | (high << (32 - shift));
        high = high >> shift;
    } else {
        low = high >> (shift - 32);
        high = (n < 0) ? 0xFFFFFFFF : 0;
    }

    if (n < 0) {
        high = ~high + 1;
        low = ~low + 1;
    }

    return ((long long)high << 32) | low;
}
