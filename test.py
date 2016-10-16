import iotaPy
import random
import sys

#create an input array 
state = []
for x in range(729) :
   n = random.randint(-1,1)
   state.append(n)

#call the function 10'000 times to measure the average time
idx = 10000
transformed_state = []
while idx > 0 :
    transformed_state = iotaPy.transform(state)
    idx -= 1

#Maunual verification of result
#print state
#print "---------------------"
#print transformed_state
