#include "knn.decl.h"
#include "kernel.h"

/*readonly*/ CProxy_KNN  knnProxy;
/*readonly*/ CProxy_Main mainProxy;

class Main : public CBase_Main {
public:
  Main(CkArgMsg *msg) {
    if (msg->argc <= 1) {
      CkPrintf("Usage: knn <inputfile>\n");
      CkExit();
    }

    int m, n, k, *V;
    FILE *fp;

    if((fp = fopen(msg->argv[1], "r")) == NULL) {
      CkPrintf("Error open input file!\n");
      CkExit();
    }

    fscanf(fp, "%d %d %d", &m, &n, &k);

    V = new int[m*n];

    for(int i = 0; i < m*n; i++) {
      fscanf(fp, "%d", &V[i]);
    }

    fclose(fp);

    mainProxy = thisProxy;
    knnProxy  = CProxy_KNN::ckNew(m, n, k, V, CkNumPes());

//    delete V;
  }
};

class KNN : public CBase_KNN {
  int m, n, k, *V, *out;
public:
  KNN(CkMigrateMessage *m) {}

  KNN(int m_, int n_, int k_, int *V_)
    : m(m_), n(n_), k(k_), V(V_) {
    thisProxy[thisIndex].start();
  }

  void start(void) {
    CkCallback *cb = new CkCallback(CkIndex_KNN::done(), CkArrayIndex1D(thisIndex), thisArrayID);

    kernelSetup(cb, thisIndex, m, n, k, V, out);
  }

  void done() {
    showResult();

    contribute(CkCallback(CkCallback::ckExit));
  }
  
  void showResult() {
  	for(int i=0; i<m; i++) {
  		for(int j=0; j<k; j++) {
  			printf("%d ", out[i*k+j]);
  			if(j == k-1) {
  				printf("\n");
  			}
  		}
  	}
  }
};

#include "knn.def.h"
