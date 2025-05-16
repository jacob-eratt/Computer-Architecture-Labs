; Test Case for JMP, JSR, and JSRR instructions

        .ORIG x3000          ; Start of the program
        JMP R3               ; Jump to the address stored in R3
        JSR TARGET           ; Jump to subroutine at label TARGET
        JSR #4
        JSRR R4              ; Jump to subroutine address in R4

TARGET  HALT                 ; Label for JSR
        .END                 ; End of the program
