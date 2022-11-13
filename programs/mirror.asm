[bits 32]
[global _start]

; A simple program that promptr you to type a character and then repeats the character back to you.

section .text

print_prompt:
	mov eax, 4
	mov ebx, 1
	mov ecx, prompt
	mov edx, 18
	int 0x80

wait_for_input:
	mov eax, 3
	mov ebx, 0
	mov ecx, buf
	mov edx, 1
	int 0x80

	cmp eax, 0
	je wait_for_input

print_input:
	mov al, [buf]
	mov [response + 12], al

	mov eax, 4
	mov ebx, 1
	mov ecx, response
	mov edx, 14
	int 0x80

_exit:
	mov eax, 1
	int 0x80

section .data

prompt:
	db `Type a character: `
response:
	db `\nYou typed: \n'
buf:
	db 0