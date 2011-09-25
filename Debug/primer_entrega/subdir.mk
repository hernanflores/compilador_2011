################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../primer_entrega/error.o \
../primer_entrega/lex.yy.o \
../primer_entrega/parser.o \
../primer_entrega/ts.o 

C_SRCS += \
../primer_entrega/error.c \
../primer_entrega/lex.yy.c \
../primer_entrega/parser.c \
../primer_entrega/ts.c 

OBJS += \
./primer_entrega/error.o \
./primer_entrega/lex.yy.o \
./primer_entrega/parser.o \
./primer_entrega/ts.o 

C_DEPS += \
./primer_entrega/error.d \
./primer_entrega/lex.yy.d \
./primer_entrega/parser.d \
./primer_entrega/ts.d 


# Each subdirectory must supply rules for building sources it contributes
primer_entrega/%.o: ../primer_entrega/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


