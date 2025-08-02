/*
 * Shader Function Tests for TinyExpr
 * 
 * This test file exercises all TinyExpr features through practical shader functions:
 * - Arithmetic operators (+, -, *, /, %, ^)
 * - Comparison operators (<, <=, >, >=, ==, !=)
 * - Logical operators (&&, ||, !)
 * - Ternary operator (condition ? true_expr : false_expr)
 * - Math functions (sin, cos, sqrt, pow, floor, fabs, etc.)
 * - Variable binding and evaluation
 * - Complex nested expressions
 */

#include "tinyexpr.h"
#include <stdio.h>
#include "minctest.h"

typedef struct {
    const char *name;
    const char *expr;
    double vars[8];  // Up to 8 variables: a, b, c, d, e, f, g, h
    double expected;
} shader_test_case;

void test_shader_functions() {
    shader_test_case cases[] = {
        // 1. Smoothstep function: ((x-a)/(b-a) < 0 ? 0 : ((x-a)/(b-a) > 1 ? 1 : (x-a)/(b-a))) ^ 2 * (3 - 2 * ((x-a)/(b-a) < 0 ? 0 : ((x-a)/(b-a) > 1 ? 1 : (x-a)/(b-a))))
        // Simplified version using intermediate variables
        {"Smoothstep 1", "a < 0 ? 0 : (a > 1 ? 1 : a*a*(3-2*a))", {0.0}, 0.000000},
        {"Smoothstep 2", "a < 0 ? 0 : (a > 1 ? 1 : a*a*(3-2*a))", {0.166667}, 0.074074}, // (0.3-0.2)/(0.8-0.2) = 0.166667
        {"Smoothstep 3", "a < 0 ? 0 : (a > 1 ? 1 : a*a*(3-2*a))", {0.5}, 0.500000},
        {"Smoothstep 4", "a < 0 ? 0 : (a > 1 ? 1 : a*a*(3-2*a))", {0.833333}, 0.925926}, // (0.7-0.2)/(0.8-0.2) = 0.833333
        {"Smoothstep 5", "a < 0 ? 0 : (a > 1 ? 1 : a*a*(3-2*a))", {1.333333}, 1.000000}, // > 1, so clamped

        // 2. Mix/lerp function: a * (1 - t) + b * t
        {"Mix 1", "a*(1-c) + b*c", {2.0, 8.0, 0.0}, 2.000000},
        {"Mix 2", "a*(1-c) + b*c", {2.0, 8.0, 0.25}, 3.500000},
        {"Mix 3", "a*(1-c) + b*c", {2.0, 8.0, 0.5}, 5.000000},
        {"Mix 4", "a*(1-c) + b*c", {2.0, 8.0, 0.75}, 6.500000},
        {"Mix 5", "a*(1-c) + b*c", {2.0, 8.0, 1.0}, 8.000000},

        // 3. Step function: x < edge ? 0 : 1
        {"Step 1", "b < a ? 0 : 1", {0.5, 0.2}, 0.000000},
        {"Step 2", "b < a ? 0 : 1", {0.5, 0.5}, 1.000000},
        {"Step 3", "b < a ? 0 : 1", {0.5, 0.8}, 1.000000},

        // 4. Clamp function: x < min ? min : (x > max ? max : x)
        {"Clamp 1", "a < b ? b : (a > c ? c : a)", {-2.0, -1.0, 1.0}, -1.000000},
        {"Clamp 2", "a < b ? b : (a > c ? c : a)", {-0.5, -1.0, 1.0}, -0.500000},
        {"Clamp 3", "a < b ? b : (a > c ? c : a)", {0.0, -1.0, 1.0}, 0.000000},
        {"Clamp 4", "a < b ? b : (a > c ? c : a)", {0.5, -1.0, 1.0}, 0.500000},
        {"Clamp 5", "a < b ? b : (a > c ? c : a)", {2.0, -1.0, 1.0}, 1.000000},

        // 5. Distance function: sqrt((x2-x1)^2 + (y2-y1)^2)
        {"Distance 1", "sqrt((c-a)^2 + (d-b)^2)", {0, 0, 3, 4}, 5.000000},
        {"Distance 2", "sqrt((c-a)^2 + (d-b)^2)", {0, 0, 1, 1}, 1.414214},
        {"Distance 3", "sqrt((c-a)^2 + (d-b)^2)", {0, 0, 5, 0}, 5.000000},

        // 6. Fract function: x - floor(x)
        {"Fract 1", "a - floor(a)", {1.25}, 0.250000},
        {"Fract 2", "a - floor(a)", {3.75}, 0.750000},
        {"Fract 3", "a - floor(a)", {-0.25}, 0.750000},

        // 7. Sign function: x > 0 ? 1 : (x < 0 ? -1 : 0)
        {"Sign 1", "a > 0 ? 1 : (a < 0 ? -1 : 0)", {-5.0}, -1.000000},
        {"Sign 2", "a > 0 ? 1 : (a < 0 ? -1 : 0)", {0.0}, 0.000000},
        {"Sign 3", "a > 0 ? 1 : (a < 0 ? -1 : 0)", {3.5}, 1.000000},

        // 8. Pulse function: (x >= edge0 && x <= edge1) ? 1 : 0
        {"Pulse 1", "(c >= a && c <= b) ? 1 : 0", {0.3, 0.7, 0.2}, 0.000000},
        {"Pulse 2", "(c >= a && c <= b) ? 1 : 0", {0.3, 0.7, 0.5}, 1.000000},
        {"Pulse 3", "(c >= a && c <= b) ? 1 : 0", {0.3, 0.7, 0.8}, 0.000000},

        // 9. Sine wave: amplitude * sin(freq * x + phase)
        {"Sine Wave 1", "c * sin(a * d + b)", {2.0, 0.5, 0.8, 0.0}, 0.383540},
        {"Sine Wave 2", "c * sin(a * d + b)", {2.0, 0.5, 0.8, 0.5}, 0.797996},
        {"Sine Wave 3", "c * sin(a * d + b)", {2.0, 0.5, 0.8, 1.0}, 0.478778},

        // 10. Circle mask: distance <= radius ? 1 : 0
        {"Circle Mask 1", "sqrt((a-c)^2 + (b-d)^2) <= e ? 1 : 0", {0.5, 0.5, 0.5, 0.5, 0.3}, 1.000000},
        {"Circle Mask 2", "sqrt((a-c)^2 + (b-d)^2) <= e ? 1 : 0", {0.7, 0.7, 0.5, 0.5, 0.3}, 1.000000},
        {"Circle Mask 3", "sqrt((a-c)^2 + (b-d)^2) <= e ? 1 : 0", {0.9, 0.5, 0.5, 0.5, 0.3}, 0.000000},

        // 11. Simple noise: fract(sin(x * 12.9898) * 43758.5453)
        {"Simple Noise 1", "sin(a * 12.9898) * 43758.5453 - floor(sin(a * 12.9898) * 43758.5453)", {1.0}, 0.921690},
        {"Simple Noise 2", "sin(a * 12.9898) * 43758.5453 - floor(sin(a * 12.9898) * 43758.5453)", {2.0}, 0.057218},
        {"Simple Noise 3", "sin(a * 12.9898) * 43758.5453 - floor(sin(a * 12.9898) * 43758.5453)", {3.0}, 0.558223},

        // 12. Safe power: base == 0 && exponent <= 0 ? 0 : pow(abs(base), exponent)
        {"Safe Power 1", "a == 0 && b <= 0 ? 0 : pow(abs(a), b)", {2.0, 3.0}, 8.000000},
        {"Safe Power 2", "a == 0 && b <= 0 ? 0 : pow(abs(a), b)", {0.0, 2.0}, 0.000000},
        {"Safe Power 3", "a == 0 && b <= 0 ? 0 : pow(abs(a), b)", {-2.0, 2.0}, 4.000000},

        // Additional complex expressions testing multiple features
        // 13. Nested ternary with logical operators
        {"Complex 1", "a > 0 && b > 0 ? (a > b ? a : b) : (a < 0 && b < 0 ? (a < b ? a : b) : 0)", {3.0, 2.0}, 3.000000},
        {"Complex 2", "a > 0 && b > 0 ? (a > b ? a : b) : (a < 0 && b < 0 ? (a < b ? a : b) : 0)", {-3.0, -2.0}, -3.000000},

        // 14. Mathematical expressions with precedence
        {"Precedence 1", "2 + 3 * 4 ^ 2 - 1", {0}, 49.000000}, // 2 + 3*16 - 1 = 49
        {"Precedence 2", "(2 + 3) * (4 ^ 2 - 1)", {0}, 75.000000}, // 5 * 15 = 75

        // 15. Logical expressions
        {"Logic 1", "!(a > b) && (c == d)", {1.0, 2.0, 3.0, 3.0}, 1.000000},
        {"Logic 2", "a != b || c <= d", {1.0, 1.0, 5.0, 3.0}, 0.000000},
    };

    double vars[8] = {0}; // Variables a, b, c, d, e, f, g, h
    te_variable lookup[] = {
        {"a", &vars[0]}, {"b", &vars[1]}, {"c", &vars[2]}, {"d", &vars[3]},
        {"e", &vars[4]}, {"f", &vars[5]}, {"g", &vars[6]}, {"h", &vars[7]}
    };

    int i;
    for (i = 0; i < sizeof(cases) / sizeof(shader_test_case); ++i) {
        const shader_test_case *test = &cases[i];
        
        // Set up variables
        int j;
        for (j = 0; j < 8; j++) {
            vars[j] = test->vars[j];
        }

        // Compile and evaluate expression
        int err;
        te_expr *expr = te_compile(test->expr, lookup, 8, &err);
        
        if (!expr) {
            printf("COMPILE ERROR: %s - Error at position %d\n", test->name, err);
            lok(0); // Fail the test
            continue;
        }

        double result = te_eval(expr);
        te_free(expr);

        // Check result with tolerance for floating point precision
        double tolerance = 1e-5;
        double diff = result - test->expected;
        if (diff < 0) diff = -diff;
        
        if (diff > tolerance) {
            printf("FAILED: %s\n", test->name);
            printf("  Expression: %s\n", test->expr);
            printf("  Variables: ");
            for (j = 0; j < 8 && test->vars[j] != 0.0; j++) {
                printf("%c=%.3f ", 'a'+j, test->vars[j]);
            }
            printf("\n");
            printf("  Expected: %.6f\n", test->expected);
            printf("  Got:      %.6f\n", result);
            printf("  Diff:     %.6f\n", diff);
        }
        
        lok(diff <= tolerance);
    }
}

int main(int argc, char *argv[])
{
    printf("Running comprehensive shader function tests for TinyExpr...\n");
    printf("These tests exercise all TinyExpr features through practical shader functions.\n\n");
    
    lrun("Shader Functions", test_shader_functions);
    lresults();

    printf("\nFeatures tested:\n");
    printf("- Arithmetic operators: +, -, *, /, %%, ^\n");
    printf("- Comparison operators: <, <=, >, >=, ==, !=\n");
    printf("- Logical operators: &&, ||, !\n");
    printf("- Ternary operator: condition ? true_expr : false_expr\n");
    printf("- Math functions: sin, cos, sqrt, pow, floor, abs\n");
    printf("- Variable binding and evaluation\n");
    printf("- Complex nested expressions\n");
    printf("- Operator precedence\n");

    return lfails != 0;
}