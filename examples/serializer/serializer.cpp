#include <Socket/Serializer.hpp>
#include <iostream>

int main(int argc,char* argv[])
{
    ntw::Serializer stream;
    std::string filename = "test-serialize.bin";

    int i = 42;
    std::string str = "test";
    char buffer[255] = "buffer";

    {
        std::cout<<"=== serialize ==="<<std::endl;
        stream<<i
            <<str;

        stream.write(buffer,255);
        std::cout<<stream<<std::endl;
    }
    {
        std::cout<<"=== save in file ==="<<std::endl;
        std::cout<<"Save : "<<((stream.save(filename)==true)?"Ok":"Failed")<<std::endl;
        std::cout<<stream<<std::endl;
    }

    auto f_unser = [&](){
        std::cout<<"=== unserialize ==="<<std::endl;

        i= 0;
        str="";
        for(int i=0;i<255;++i)
            buffer[i] = 0;

        stream>>i
            >>str;
        stream.read(buffer,255);

        std::cout<<"i: "<<i<<" expected: 42"<<std::endl
            <<"str: "<<str<<" expected: test"<<std::endl
            <<"buffer: "<<buffer<<" expected: buffer"<<std::endl;
        std::cout<<stream<<std::endl;
    };
    f_unser();

    {
        std::cout<<"=== clear stream + load ==="<<std::endl;
        stream.clear();
        std::cout<<stream<<std::endl;
        std::cout<<"load : "<<((stream.load(filename)==true)?"Ok":"Failed")<<std::endl;
        std::cout<<stream<<std::endl;
    }
    f_unser();

}
