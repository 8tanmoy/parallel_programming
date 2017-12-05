#Tanmoy Pal, University of Wisconsin Madison

import numpy as np
import math
import time

n = 1000
I = np.eye(n,n)
A = np.zeros((n,n))
for i in range(n):
    A[i][i] = (i+1)*10
A = A + 0.001*np.random.randn(n,n)
A = (A.T + A)/2
k=0

np_i = time.time()
eigenVals = np.linalg.eig(A)[0]
idxm = eigenVals.argsort()
eigenVals = eigenVals[idxm]
lambdak = eigenVals[k]
np_e = time.time()

print("Numpy eigenvalue no. "+str(k+1)+" : "+str(lambdak))
print()
print("\t\tDavidson")

d_i = time.time()
l= k + 4
b = np.eye(n,l)
Ab = np.dot(A,b)
At = np.dot(b.T,Ab)
E,V = np.linalg.eig(At)
idx1 = E.argsort()[::1]
E = E[idx1]
V = V[:,idx1]
lam = E[k]
alp = V[:,k]

print("eigenvalue no. "+str(k+1)+" in step 1 is " + str(lam))
print("error in step 1 is " + str(abs(lambdak-lam)))
print()

error = 0
error_old = abs(lambdak-lam)
step = 1

M=l
while (M < n//2):
    step = step + 1
    q = np.zeros(n)
    ksi = np.zeros(n)
    d = np.zeros(n)
    bn = np.zeros(n)
    Abn = np.zeros(n)
    Atn = np.zeros(((M+1),(M+1)))
    mult = np.eye(n,n)
    temp1 = 0
    temp2 = np.zeros((n,n))
    sum1 = np.zeros(n)
    sum2 = np.zeros(n)
    for j in range(M):
        sum1 = sum1 + np.multiply([alp[j]],Ab[:,j])
        temp1 = (alp[j]*lam)
        sum2 = sum2 + np.multiply([temp1],b[:,j])
    q = sum1 - sum2
    for i in range(n):
        ksi[i] = (1/(lam - A[i][i]))*q[i]
    for j in range(M):
        temp2 = I - np.outer(b[:,j],b[:,j])
        mult = np.multiply(mult,temp2)
    d = np.dot(mult,ksi)
    bn = np.multiply([(1/(np.linalg.norm(d)))],d)
    Abn = np.dot(A,bn)
    temp3 = np.zeros(M+1)
    for i in range(M):
        for j in range(M):
            Atn[i][j] = At[i][j]
    b = np.insert(b,M,bn,1)
    Ab = np.insert(Ab,M,Abn,1)
    for j in range(M+1):
        Atn[j][M] = np.dot(b[:,j].T,Abn)
    for i in range(M):
        Atn[M][i] = np.dot(bn.T,Ab[:,i])
    At = np.zeros(((M+1),(M+1)))
    At = Atn
    E = np.zeros(M+1)
    V = np.zeros((M+1,M+1))
    E,V = np.linalg.eig(At)
    idx = E.argsort()[::1]
    E = E[idx]
    V = V[:,idx]
    lam = E[k]
    alp = V[:,k]
    M=M+1
    error = lambdak - lam
    if abs(error) > abs(error_old):
        print("**terminated. This is the best convergence**")
        break
    if abs(error) < 1e-15:
        print("eigenvalue no. "+str(k+1)+" in step "+ str(step) + " is " + str
            (lam))
        print("error in step "+ str(step) + " is " + str(abs(lambdak-lam)))
        print()
        print("**converged**")
        print("\t\tEND")
        break
    else:
        print("eigenvalue no. "+str(k+1)+" in step "+ str(step) + " is " + str(lam))
        print("error in step "+ str(step) + " is " + str(abs(lambdak-lam)))
        print()
    error_old = error
d_e = time.time()
print("\t\tTIME DATA")
print("numpy:\t"+str(np_e - np_i)+" seconds")
print("Davidson:\t"+str(d_e - d_i)+" seconds")
