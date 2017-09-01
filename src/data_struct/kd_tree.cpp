#include <cstdio>
#include <iostream>
#define DEBUG_LINE printf("%s:%d\n", __FILE__, __LINE__)
#define min(x,y) ((x)<(y)?(x):(y))
#define abs(x) ((x)<0?(-(x)):(x))
#include <vector>

const int MAXLONGINT = 0x7fffffff;

//�����࣬��ά�ռ��еĵ�
class myVector{
public:
	int d;
	int * buf;
	
	myVector(int n) : d(n){
		buf = new int[d];
	}

	myVector(){
		d = 0;
		buf = NULL;
	}

	myVector(const myVector & _data){
		d = _data.d;
		buf = new int[d];
		for (int i = 0; i < d; ++i) {
			buf[i] = _data.buf[i];
		}
	}

	int * setD(int _d){
		d = _d;
		if (buf) {
			delete[] buf;
		}
		return buf = new int[d];
	}

	int output() const{
		printf("{");
		for (int j = 0; j < d; ++j){
			int flag = d == j + 1;
			printf(" %d%c", buf[j], (flag)?' ':','); 
		}
		puts("}");
	}

	int dist(const myVector& a)const {
		int ret = 0, _d = (d, a.d);
		for (int i = 0; i < _d; ++i) {
			int t = a.buf[i] - buf[i];
			ret += t*t;
		}
		return ret;
	}

	int minDist(const myVector& a) const {
		int ret = MAXLONGINT, _d = (d, a.d);
		for (int i = 0; i < _d; ++i) {
			ret = min(ret, abs(buf[i] - a.buf[i]));
		}
		return ret;
	}

	bool operator == (const myVector&a) const {
		if (a.d != d) return false;
		for (int i = 0; i < d; ++i) {
			if (buf[i] != a.buf[i]) return false;
		}
		return true;
	}

	bool operator != (const myVector&a) const {
		if (a.d == d) return false;
		for (int i = 0; i < d; ++i) {
			if (buf[i] == a.buf[i]) return false;
		}
		return true;
	}

	myVector operator = (const myVector & _data){
		d = _data.d;
		if (buf) delete buf;
		buf = new int[d];
		for (int i = 0; i < d; ++i) {
			buf[i] = _data.buf[i];
		}
		return *this;
	}
	
	bool operator > (const myVector&a) const {
		if (a.d != d) return false;
		for (int i = 0; i < d; ++i) {
			if (buf[i] <= a.buf[i]) return false;
		}
		return true;
	}

	bool operator < (const myVector&a) const {
		if (a.d != d) return false;
		for (int i = 0; i < d; ++i) {
			if (buf[i] >= a.buf[i]) return false;
		}
		return true;
	}

	bool operator >= (const myVector&a) const {
		if (a.d != d) return false;
		for (int i = 0; i < d; ++i) {
			if (buf[i] < a.buf[i]) return false;
		}
		return true;
	}

	bool operator <= (const myVector&a) const {
		if (a.d != d) return false;
		for (int i = 0; i < d; ++i) {
			if (buf[i] > a.buf[i]) return false;
		}
		return true;
	}


	~myVector(){
		if (buf) delete buf;
	}
};

//KD���ڵ㶨��
struct KDnode{
	myVector * data;	//����ָ��
	KDnode * left;		//�����ָ��
	KDnode * right;		//�Ҷ���ָ��
};

//Ѱ�ҵ�k����������ֱ�������м��ֵ
//�㷨˼�����������ʽ����
//C++ STL���Ѿ���ʵ����
int find_middle(int * buf, const int L, const int R, const int k){
	if (L == R) return buf[L];
	int l = L, r = R;
	int mid = buf[(L + R) >> 1];
	do {
		while (buf[l] < mid) ++l;
		while (buf[r] > mid) --r;
		if (l <= r) {
			int tmp = buf[l];
			buf[l] = buf[r];
			buf[r] = tmp;
			++l, --r;
		}
	} while (l <= r);

	if (k <= r) return find_middle(buf, L, r, k);
	if (k >= l) return find_middle(buf, l, R, k);
	return buf[r+1];
}

//KD������
KDnode * build_kdTree(myVector * data, const int n, const int d, const int depth){
	//�½���ʼ��
	if (n == 0) return NULL;
	KDnode * root = new KDnode;
	root->data = NULL;
	root->left = NULL;
	root->right = NULL;

	int k = depth % d;
	int * tmp = new int[n];
	for (int i = 0; i < n; ++i) {
		tmp[i] = data[i].buf[k];
	}
	
	int mid = find_middle(tmp, 0, n-1, ((n-1)>>1));
	delete tmp;
	
	//ʹ�õ�kά���м�ֵ��Ϊ�������ݣ��������ĸ߶�
	int ln = 0, rn;
	for (int i = 0; i < n; ++i) {
		if (data[i].buf[k] < mid) ++ln;
	}
	rn = (n-1) - ln;

	myVector * leftD = NULL, * rightD = NULL;
	if (ln != 0) leftD = new myVector[ln];
	if (rn != 0) rightD = new myVector[rn];
	for (int i = 0; i < ln; ++i) leftD[i].setD(d);
	for (int i = 0; i < rn; ++i) rightD[i].setD(d);

	int lc = 0, rc = 0;
	for (int i = 0; i < n; ++i) {
		if (data[i].buf[k] < mid) {
			leftD[lc++] = data[i];
		} else if (data[i].buf[k] == mid && root->data == NULL){
			root->data = new myVector(data[i]);
		} else {
			rightD[rc++] = data[i];
		}
	}
	//�ݹ鴴������
	if (ln) root->left = build_kdTree(leftD, ln, d, depth+1);
	if (rn) root->right = build_kdTree(rightD, rn, d, depth+1);
	if (leftD) delete []leftD;
	if (rightD) delete []rightD;

	return root;
}

void treeOutput(const KDnode * root, const int d, const int depth){
	int k = depth % d;
	if (root->left) treeOutput(root->left, d, depth+1);
	for (int i = 0; i < depth; ++i) {
		printf("    ");
	}
	root->data->output();
	if (root->right) treeOutput(root->right, d, depth+1);
}

//������������������ȡ�����
myVector treeDFS(const KDnode * root, const myVector & data, const int depth, int & minDist){
	int k = depth % (root->data->d);
	myVector tmp, best = *root->data;
	int tmpDist, bestDist;
	bestDist = min(minDist, data.dist(*root->data));
	tmpDist = MAXLONGINT;
	if (root->left) tmp = treeDFS(root->left, data, depth+1, tmpDist);
	if (tmpDist < bestDist) {
		bestDist = tmpDist;
		best = tmp;
	}
	tmpDist = MAXLONGINT;
	if (root->right) tmp = treeDFS(root->right, data, depth+1, tmpDist);
	if (tmpDist < bestDist) {
		bestDist = tmpDist;
		best = tmp;
	}
	return best;
}

//Ѱ������ڵ�
myVector findNN(KDnode * root, const myVector & data, const int depth, int & minDist){
	if (root == NULL) return myVector();
	if (*(root->data) == data) {
		minDist = 0;
		return *(root->data);
	}

	int k = depth % (data.d);
	int mid = root->data->buf[k];
	
	myVector best = *(root->data), tmp;
	minDist = min(minDist, data.dist(best));
	int tmpDist = MAXLONGINT;

	if (data.buf[k] < mid) {
		if (root->left) tmp = findNN(root->left, data, depth+1, tmpDist);
	} else {
		if (root->right) tmp = findNN(root->right, data, depth+1, tmpDist);
	}

	if (tmpDist < minDist){
		minDist = tmpDist;
		best = tmp;
	}
	
	//�������ѯ����߽�ľ���С�����ž��룬����Ҫ������������
	if (minDist > data.minDist(*root->data)) {
		KDnode * proot = root->left;
		if (data.buf[k] < mid) {
			proot = root->right;
		}
		if (proot) {
			tmp = treeDFS(proot, data, depth+1, tmpDist);
		}	
	}

	return best;
}

//������
KDnode * insertNode(KDnode * root, const myVector & data, const int depth){
	//����Ѿ���Ҷ�ӽڵ�֮�£������½��
	if (root == NULL) {
		root = new KDnode;
		root->data = new myVector(data);
		root->left = NULL;
		root->right = NULL;
		return root;
	}
	if (data == *root->data) {
		return NULL;
	}
	int d = data.d;
	int k = depth % d;
	if (data.buf[k] < root->data->buf[k]) {
		root->left = insertNode(root->left, data, depth+1);
	} else {
		root->right = insertNode(root->right, data, depth+1);
	}
	return root;
}

//��Χ��ѯ
int rangeQ(KDnode * root, const myVector & low, const myVector & high, const int depth, std::vector<myVector> &res){
	int resn = 0;
	if (root == NULL) return resn;

	if (high < low) return resn;
	//����õ����Ҫ�󣬼�����һ
	if (low <= *root->data && *root->data <= high) {
		resn++;
		res.push_back(*root->data);
	}

	int k = depth % root->data->d;
	//����������з�������
	if (root->data->buf[k] >= low.buf[k] && root->data->buf[k] <= high.buf[k]) {
		resn += rangeQ(root->left, low, high, depth+1, res);
		resn += rangeQ(root->right, low, high, depth+1, res);
	} else if (root->data->buf[k] < low.buf[k]) {
		resn += rangeQ(root->right, low, high, depth+1, res);
	} else if (root->data->buf[k] > high.buf[k]) {
		resn += rangeQ(root->left, low, high, depth+1, res);
	}
	return resn;
}

int main(){
	int n, d;
	printf("please input the number of dimension: ");
	scanf("%d", &d);
	printf("please input the number of data: ");
	scanf("%d", &n);

	myVector * data = new myVector[n];	

	for (int i = 0; i < n; ++i){
		data[i].setD(d);
		for (int j = 0; j < d; ++j) {
			scanf("%d", &data[i].buf[j]);
		}
	}

	KDnode * root = build_kdTree(data, n, d, 0);
	puts("kd-tree����ͼ: ");
	treeOutput(root, d, 0);
	puts("");

	int minDist = MAXLONGINT;
	myVector fa(d);
	myVector high(d);

	int opt = 0;
	printf("����0��ʾ��ѯ�����, ����1��ʾ����µ�, ����2��ʾ���з�Χ��ѯ: ");
	while (scanf("%d", &opt) > 0){
		if (opt == 0) printf("������Ҫ��ѯ�ĵ�: ");
		else if (opt == 1)printf("������Ҫ��ӵĵ�: ");
		else printf("������Ҫ��ѯ�ķ�Χ: ");
		for (int i = 0; i < d; ++i) {
			scanf("%d", &fa.buf[i]);
		}
		if (opt == 2)
			for (int i = 0; i < d; ++i) {
			scanf("%d", &high.buf[i]);
		}

		if (opt == 0) {
			printf("����ĵ�Ϊ: ");
			findNN(root, fa, 0, minDist).output();
		} else if (opt == 1) {
			insertNode(root, fa, 0);
			puts("��kd-tree����ͼ: ");
			treeOutput(root, d, 0);
		} else {
			std::vector<myVector> res;
			printf("����Ҫ��ĵ��� %d ��, �ֱ�Ϊ: \n", rangeQ(root, fa, high, 0, res));
			for (int i = 0; i < res.size(); ++i){
				res[i].output();
			}
		}
		printf("\n����0��ʾ��ѯ�����, ����1��ʾ����µ�, ����2��ʾ���з�Χ��ѯ: ");
	}

	delete []data;
	return 0;
}
