long long __aeabi_llsr(long long n, int shift) {
    if (shift < 0 || shift >= 64) {
        // Handle invalid shift value
        // You can either return an error or abort the program
        // For this example, we'll just return 0
        return 0;
    }

    unsigned int high = (unsigned int)(n >> 32);
    unsigned int low = (unsigned int)n;

    if (shift < 32) {
        low = (low >> shift) | (high << (32 - shift));
        high = high >> shift;
    } else {
        low = high >> (shift - 32);
        high = 0;
    }

    return ((long long)high << 32) | low;
}
