#include <stdint.h>

// Function prototypes (assuming these exist in your codebase)
extern void cell12_Temp_01_Set(uint8_t temperature);
extern void cell12_Temp_02_Set(uint8_t temperature);
extern void cell12_Temp_03_Set(uint8_t temperature);
extern void cell11_Temp_01_Set(uint8_t temperature);
extern void cell11_Temp_02_Set(uint8_t temperature);
extern void cell11_Temp_03_Set(uint8_t temperature);

// OPTIMIZATION 1: Function pointer array approach
// This is the most efficient and maintainable solution

typedef void (*temp_set_func_t)(uint8_t temperature);

// Function pointer lookup table
static const temp_set_func_t temp_set_functions[] = {
    NULL,                // Index 0 (invalid)
    cell12_Temp_01_Set,  // Index 1 (0x01)
    cell12_Temp_02_Set,  // Index 2 (0x02)
    cell12_Temp_03_Set,  // Index 3 (0x03)
    cell11_Temp_01_Set,  // Index 4 (0x04)
    cell11_Temp_02_Set,  // Index 5 (0x05)
    cell11_Temp_03_Set   // Index 6 (0x06)
};

#define MAX_TEMP_CARD_ID (sizeof(temp_set_functions) / sizeof(temp_set_functions[0]) - 1)

// Optimized version with function pointer array
void Set_Resistance_Optimized_V1(uint8_t tempCardId, uint8_t temperature)
{
    // Input validation and direct function call
    if (tempCardId > 0 && tempCardId <= MAX_TEMP_CARD_ID && temp_set_functions[tempCardId] != NULL) {
        temp_set_functions[tempCardId](temperature);
    }
    // Optionally handle invalid IDs (uncomment if needed):
    // else {
    //     // Handle error - invalid card ID
    // }
}

// OPTIMIZATION 2: Pattern-based approach
// This exploits the naming pattern in your functions

void Set_Resistance_Optimized_V2(uint8_t tempCardId, uint8_t temperature)
{
    // Input validation
    if (tempCardId < 0x01 || tempCardId > 0x06) {
        return; // Invalid card ID
    }
    
    // Pattern-based function selection
    if (tempCardId <= 0x03) {
        // cell12 functions (0x01, 0x02, 0x03)
        switch (tempCardId) {
            case 0x01: cell12_Temp_01_Set(temperature); break;
            case 0x02: cell12_Temp_02_Set(temperature); break;
            case 0x03: cell12_Temp_03_Set(temperature); break;
        }
    } else {
        // cell11 functions (0x04, 0x05, 0x06)
        switch (tempCardId - 0x03) {
            case 0x01: cell11_Temp_01_Set(temperature); break;
            case 0x02: cell11_Temp_02_Set(temperature); break;
            case 0x03: cell11_Temp_03_Set(temperature); break;
        }
    }
}

// OPTIMIZATION 3: Most compact version (if function naming allows)
// This assumes you can create arrays of function pointers for each cell type

static const temp_set_func_t cell12_functions[] = {
    cell12_Temp_01_Set,
    cell12_Temp_02_Set,
    cell12_Temp_03_Set
};

static const temp_set_func_t cell11_functions[] = {
    cell11_Temp_01_Set,
    cell11_Temp_02_Set,
    cell11_Temp_03_Set
};

void Set_Resistance_Optimized_V3(uint8_t tempCardId, uint8_t temperature)
{
    if (tempCardId >= 0x01 && tempCardId <= 0x03) {
        // cell12 functions
        cell12_functions[tempCardId - 0x01](temperature);
    } else if (tempCardId >= 0x04 && tempCardId <= 0x06) {
        // cell11 functions  
        cell11_functions[tempCardId - 0x04](temperature);
    }
    // Invalid IDs are silently ignored
}

/*
OPTIMIZATION BENEFITS:

1. **Performance**: 
   - V1: O(1) lookup time vs O(n) switch statement
   - V3: Eliminates multiple comparisons, direct array access

2. **Memory**: 
   - Function pointer arrays use less code space than switch statements
   - Better cache locality

3. **Maintainability**:
   - Easy to add new temperature cards (just extend the array)
   - Clear separation of concerns
   - Less code duplication

4. **Safety**:
   - Input validation prevents crashes from invalid IDs
   - NULL pointer checks in V1

5. **Readability**:
   - Intent is clearer with lookup tables
   - Pattern recognition makes logic explicit

RECOMMENDATION:
Use V1 (function pointer array) for best balance of performance, 
maintainability, and safety. Use V3 if you want maximum compactness 
and performance is critical.
*/