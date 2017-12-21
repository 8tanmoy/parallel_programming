#davidson's method serial python
import numpy as np
import math
import time

#print header
print("#size\t" + "#ev_no\t" + "#step\t"+ "#val\t\t" + "#time(ms)")

for ex in range(3,15):
    #ex = 5
    n = 2**ex
    I = np.eye(n,n)
    A = np.zeros((n,n))
    for i in range(n):
        A[i][i] = (i+1)*1
    A = A + 0.001	#*np.random.randn(n,n)
    A = (A.T + A)/2
    k = 0

    d_i = time.time()		#start time

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

    step = 1
    lam_old = lam;

    M = l
    while (M < n):
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

    #--Weinstein lower bound convergence check--   
        if (lam < abs(lam - np.linalg.norm(q))):
            print(str(n)+"\t"+str(k)+"\t"+str(step-1)+"\t"+str(lam_old)+"\t", end='')
            break
        else:
            lam_old = lam

        step = step + 1

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

        M = M+1
        
    d_e = time.time()
    print(str((d_e - d_i)*1000))
