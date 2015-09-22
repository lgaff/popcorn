section .init
        ; crtend.o goes here allegedly...
        pop ebp
        ret

section .fini
        ; crtend.o .fini goes here (again, allegedly. not sure if clang will handle this right)
        pop ebp
        ret
        
        

        
        
        
