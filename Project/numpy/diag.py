import numpy as np
import math
import time

#print header
print("matsize\t" + "#ev no.\t" + "#val\t\t" + "#time(ms)")

for ex in range(3,11):
    n = 2**ex
    A = np.zeros((n,n))
    for i in range(n):
        A[i][i] = (i+1)*1
    A = A + 0.001 #*np.random.randn(n,n)
    A = (A.T + A)/2
    k = 0
    
    np_i = time.time()
    eigenVals = np.linalg.eig(A)[0]
    idxm = eigenVals.argsort()
    eigenVals = eigenVals[idxm]
    lambdak = eigenVals[k]
    np_e = time.time()
    
    print(str(n)+"\t"+str(k)+"\t"+str(lambdak)+"\t"+str((np_e - np_i)*1000))

