################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../segunda_entrega/error.o \
../segunda_entrega/lex.yy.o \
../segunda_entrega/parser.o \
../segunda_entrega/set.o \
../segunda_entrega/ts.o 

C_SRCS += \
../segunda_entrega/error.c \
../segunda_entrega/lex.yy.c \
../segunda_entrega/parser.c \
../segunda_entrega/set.c \
../segunda_entrega/ts.c 

OBJS += \
./segunda_entrega/error.o \
./segunda_entrega/lex.yy.o \
./segunda_entrega/parser.o \
./segunda_entrega/set.o \
./segunda_entrega/ts.o 

C_DEPS += \
./segunda_entrega/error.d \
./segunda_entrega/lex.yy.d \
./segunda_entrega/parser.d \
./segunda_entrega/set.d \
./segunda_entrega/ts.d 


# Each subdirectory must supply rules for building sources it contributes
segunda_entrega/%.o: ../segunda_entrega/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


