section .init
        ; crtend.o goes here allegedly...
        pop rbp
        ret

section .fini
        ; crtend.o .fini goes here (again, allegedly. not sure if clang will handle this right)
        pop rbp
        ret
        
        

        
        
        
