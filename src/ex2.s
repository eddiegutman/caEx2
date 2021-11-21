// 311432082 Edward Gutman
        .text
.globl  even
        .type   even, @function
even:
        movl    %edi, %eax      # move num to the return register %rax.
        movl    %esi, %ecx      # move i to the shifting register %rcx.
        sall    %cl, %eax       # left shift num(%rax) by i(%rsi).
        ret

.globl  go
        .type   go, @function
go:
        # setup
        push    %rbp            # save the old frame pointer
        movq    %rsp, %rbp      # create the new frame position

        # code
        xorl    %edx, %edx      # sum = 0.
        xorl    %esi, %esi      # i = 0.

.loopCondition:
        cmp     $10, %esi       # compare i : 10
        jge     .done
.if:
        movl    (%rdi,%rsi,4), %ecx   # move A[i] to %ecx.
        test    $1, %cl         # test lsb of A[i] with 1.
        jne     .else           # if A[i] is odd.

        push    %rdi            # save A.
        push    %rsi            # save i.
        push    %rdx            # save sum.
        push    %rcx            # save A[i].

        movl    %ecx, %edi      # move A[i] to first argument.
                                # i already in second argument.
        call    even            # call even(A[i], i).

        pop     %rcx            # restore A[i].
        pop     %rdx            # restore sum.
        pop     %rsi            # restore i.
        pop     %rdi            # restore A.

        addl    %eax, %edx      # sum += num.

        incl    %esi            # i++.
        jmp     .loopCondition
.else:
        addl    %ecx, %edx      # sum += A[i].
        incl    %esi            # i++.
        jmp     .loopCondition
.done:
        # return
        movl    %edx, %eax      # move sum to the return register %eax.
        movq    %rbp, %rsp
        pop     %rbp
        ret
