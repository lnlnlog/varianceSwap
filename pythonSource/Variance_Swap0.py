import math

print("input required parameters\ncontent in parentheses will be taken as default input\
 if you press enter without input anything\n")

# First try to open a text file
fhandle = input("Please input the name of a text file (all_1500_2560.txt): ")
if len(fhandle) < 1: fhandle = "all_1500_2560.txt" #Press Enter could extract file

try:
    fname = open(fhandle)
except:
    print("Please input valid text file name")
    exit()

print(fhandle)

# Parameters:
r = input("Input continuously compounded annualized risk free rate (0.0178683):")  #Continuously compounded risk free annualized rate
if len(r) < 1: r = float(0.0178683)
else: r = float(r)

print(r)

T = input("Input time to maturity(in years) (28/365~0.0767123):")#Time in terms of year
if len(T) < 1: T = float(0.0767123)
else: T = float(T)

print(T)

S_bid = input("input bid for SPX (2347.89):")
if len(S_bid) < 1: S_bid = float(2347.89)
else: S_bid = float(S_bid)

print(S_bid)

S_ask=input("input ask for SPX (2348.53):")
if len(S_ask) < 1: S_ask = float(2348.53)
else: S_ask = float(S_ask)

print(S_ask)

S0 = 1/2 * (S_bid + S_ask)

strike = [] #Strike value
price_call = [] #close price value for call option
price_put = [] #close price value for put option


for i,line in enumerate(fname):
    line = line.strip()
    line_sep = line.split()
    if math.fmod(i,2) == 0:
        strike.append(float(line_sep[0]))
        price_call.append(float(line_sep[1]))
    else:
        price_put.append(float(line_sep[1]))
        

# Calculate S*
S_star_initial = S0*math.exp(r*T)
i = 0

for j in strike:
    if j > S_star_initial: break
    i = i + 1

S_star = strike[i-1]

# Select put and call price according to the S* value
slcprice_call = price_call[i-1:]
slcprice_put = price_put[0:i]
dk = strike[1]-strike[0]

# Calculate variance value using naive integration
summary1 = sum([p * dk /(k**2) for k,p in zip(strike[0:i],slcprice_put)])
summary2 = sum([c * dk /(k**2) for k,c in zip(strike[i-1:],slcprice_call)])

# fair value variance strike
K_var = 2/T * (r * T - (S0 / S_star * math.exp(r * T) - 1) - math.log(S_star / S0) + math.exp(r * T) * (summary1 + summary2))

print ("The fair value strike of variance swap is", K_var)
print ("The fair value volatility of variance swap is", math.sqrt(K_var))





