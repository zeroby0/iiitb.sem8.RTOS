import time
import random
import threading 

def Sensors(): 
    print("Sensor\n") 
    sensor1 = open(r"sensor1.txt","w")
    s1= random.randint(50,101)
    sensor1.write(str(s1)) 
    sensor1.close() #to change file access modes  

    sensor2 = open(r"sensor2.txt","w")
    s2= random.randint(50,90)
    sensor2.write(str(s2)) 
    sensor2.close() #to change file access modes  
  
    sensor3 = open(r"sensor3.txt","w")
    s3= random.randint(1000,2000)
    sensor3.write(str(s3)) 
    sensor3.close()   

    sensor4 = open(r"sensor4.txt","w")
    s4= random.randint(1500,3000)
    sensor4.write(str(s4)) 
    sensor4.close()   

    sensor5 = open(r"sensor5.txt","w")
    s5= random.randint(2000,3000)
    sensor5.write(str(s5)) 
    sensor5.close()   
    return 

while True :
    time.sleep(2) 
    Sensors()
print("Exit\n") 