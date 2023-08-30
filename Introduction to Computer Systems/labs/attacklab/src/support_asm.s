.file "support.s"
.text


.global gadget1

gadget1:
    popq %rdi
    ret


.global run_on_stack
.type run_on_stack, @function

/* run_on_stack(func, stack) */
run_on_stack:
    mov %rsp, %rdx            /* store old rsp in rdx */
    lea -0x8(%rsi), %rsp      /* switch rsp to stack-0x8 */
    pushq %rdx                /* save old rsp on new stack */
    callq *%rdi               /* call func (rsp aligned by 16) */
    popq %rdx                 /* pop old rsp from new stack */
    mov %rdx, %rsp            /* switch back to old rsp */
    retq                      /* return to callee on old rsp */
