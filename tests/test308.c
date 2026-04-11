// EXPECT: 3
// SECTION: ブロック内 return / ブロック外 return

int main(){
    {
        {
            {
                return 3;
            }
        }
    }
    return 0;
}

