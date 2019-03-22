import time as time
def print_name(prefix): 
    print("Searching prefix:{}".format(prefix)) 
    try :  
        while True: 
                name = (yield) 
                if prefix in name: 
                    print(name) 
    except GeneratorExit: 
            print("Closing coroutine!!") 

def main():
   handle = print_name("Dear")   # Call the co Routine first 
   #time.sleep(4)
   handle.__next__()             # Move to next 
   #time.sleep(6)
   handle.send("Manoj")         # Send a message to co routine 
   #time.sleep(3)
   handle.send("Dear Mahesh")    # Send a message to co routine 
   #time.sleep(4)
   handle.send("Dear Anand")     # Send a message to co routine 
   #time.sleep(7)
   handle.send("Dr Anand")       # Send a message to co routine 
   #time.sleep(8)
   handle.close()                # Close the handle

if __name__ == '__main__' :
  main()
