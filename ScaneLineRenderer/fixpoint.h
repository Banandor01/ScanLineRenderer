#ifndef FIXPOINT_H
#define FIXPOINT_H

class fixedp {
    const static int factor = 256;
public:
    // ctrs
    fixedp () {   value = 0;  }
    fixedp(const fixedp& other) { value = other.value; }
    fixedp(float v) { value = v * factor; }
    fixedp(int v) { value = v * factor; }

    // IntF oprands operators
    fixedp operator* (fixedp & other) { fixedp k; k.value = other.value * this->value / factor; return k; }
    fixedp operator/ (fixedp & other) { fixedp k; k.value = this->value *factor / other.value;  return k; }
    fixedp operator+ (fixedp & other) { fixedp k; k.value = this->value + other.value; return k; }
    fixedp operator- (fixedp & other) { fixedp k; k.value = this->value - other.value; return k; }

    // Int oprands operators
    fixedp& operator/= (int other) { this->value /= other; return *this; }

    fixedp operator/ (int other) { fixedp k; k.value = this->value / other; return k; }
    fixedp operator* (int other) { fixedp k; k.value = this->value * other; return k; }
    fixedp operator+ (int other) { fixedp k; k.value = this->value + other*factor; return k; }
    fixedp operator- (int other) { fixedp k; k.value = this->value - other*factor; return k; }

    friend int operator+ (int lv, fixedp rv) { return lv + rv.value / factor; }
    friend int operator- (int lv, fixedp rv) { return lv - rv.value / factor; }

    bool operator!= (int other) { return (other != this->value/factor); }

    float GetFloatValue() { return (float)value / 64.0f; }
    int GetInt() { return value / factor;}
    //friend IntF operator/ (IntF lv, int other);

private:
    int value;
};

#endif // FIXPOINT_H
