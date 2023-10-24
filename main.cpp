#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <stdio.h>
#include <atomic>
using namespace std;

mutex mtx;
shared_mutex smtx;

class Client {
    atomic<double> saldo = 0;
    mutex mtx;
public:
    Client() = default;
    explicit Client(double saldo): saldo(saldo) {}
    void depositar(double importe) {
        this_thread::sleep_for(chrono::milliseconds(10));
//        lock_guard lg(mtx);
        saldo += importe;
    }
    void retirar(double importe) {
        this_thread::sleep_for(chrono::milliseconds(10));
//        unique_lock ul(mtx);
        saldo -= importe;
//        ul.unlock();
//        //....
//        ul.lock();
    }
// NO DEBEN HACERLO, el scope del mutex no afecta a la variable result
//    void obtener_saldo(int& result) {
//        lock_guard lg(mtx);
//        result += saldo;
//    }
// NO DEBEN HACERLO

    friend ostream& operator<< (ostream& out, const Client& c) {
        out << c.saldo;
        return out;
    }
};

void ejercicio_3(int nd, int nr, double importe, Client& c) {
    vector<thread> vd(nd);
    vector<thread> vr(nr);
    for (auto& t : vd)
        t = thread(&Client::depositar, &c, importe);

    for (auto& t: vr)
        t = thread(&Client::retirar, &c, importe);

    for (auto& t : vd) t.join();
    for (auto& t : vr) t.join();
}

void ejercicio_4() {
    int m = 40;
    vector<Client> vc(m);
    for (auto& c: vc) {
        ejercicio_3(100, 50, 1, c);
    }

    for (auto& c: vc) cout << c << " ";
}

void leer(int& var, int id) {
    shared_lock sl(smtx);
    printf("hilo #: %d valor, %d\n", id, var);
    this_thread::sleep_for(chrono::milliseconds(2000));
}

void escribir(int& var, int inc, int id) {
    unique_lock ul(smtx);
    var += inc;
    printf("hilo #: %d valor actualizado, %d\n", id, var);
    this_thread::sleep_for(chrono::milliseconds(500));
}

void ejercicio_5() {
    int x = 10;
    thread t1(leer, ref(x), 1);
    thread t2(leer, ref(x), 2);
    thread t3(leer, ref(x), 3);
    thread t4(leer, ref(x), 4);
    thread t5(escribir, ref(x), 10, 5);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
}

recursive_mutex rmtx;

void factorial(int n, int& result) {
    if (n == 0) {
        result = 1;
        return;
    }
    unique_lock ul(rmtx);
    factorial(n - 1, result);
    result = n * result;
}

void ejercicio_6() {
    int f = 0;
    factorial(5, f);
    cout << f << endl;
}



int main() {
    Client c;
    //ejercicio_3(100, 50, 1, c);
//    cout << c;
    ejercicio_4();
//    ejercicio_5();
//    ejercicio_6();
    return 0;
}
