// EXPECT: 77
// SECTION: ブロック内 return / ブロック外 return

int main(){
    int result = 0;
    {
        int tmp = 77;
        result = tmp;
    }
    return result;
}

