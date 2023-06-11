import serial
import os
from datetime import datetime

baudRate = 9600
numLeituras = 10

#Leitura de dados
print("Qual é o local da porta serial? ")
localPorta = input()
print("Quantas leituras deseja?")
numLeituras = int(input())

#Conecta na porta serial
ser = serial.Serial(localPorta, baudRate)
print("Conexão bem sucedida!")

#Cria nome arquivo baseado no tempo de execucao do programa
time = datetime.now()
current_time = time.strftime("%Y%m%d-%H%M")
fileNameSaida = "PagateMeasure"+current_time+".csv"

#Abre o arquivo e coloca cabecalho
currentWkdir = os.getcwd()
dataFile = open(os.path.join(currentWkdir, fileNameSaida), "w")
dataFile.write("lap_num,lap_time,instant_speed\n")

linhaAtual = 1
while linhaAtual <= numLeituras:
    dados = ser.readline()
    print("Lido: ", bytes.decode(dados, "utf-8"))
    dataFile.write(bytes.decode(dados, "utf-8"))
    linhaAtual += 1



print("Leitura Concluída!")
ser.close()
dataFile.close()
