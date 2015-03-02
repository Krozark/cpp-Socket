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

    {
        std::cout<<"=== Serializer::convert ==="<<std::endl;
        const int bufsize = 512;
        char buffer[bufsize] = {0};

        long int i = 1234567890123450;;
        ntw::Serializer::convert(i,buffer);

        std::cout<<i<< ": ";
        for(int i =0; i<bufsize;++i)
            std::cout<<"<"<<buffer[i]<<">";
        std::cout<<std::endl;

        i = 0;
        ntw::Serializer::convert(buffer,i);

        std::cout<<i<< ": ";
        for(int i =0; i<bufsize;++i)
            std::cout<<"<"<<buffer[i]<<">";
        std::cout<<std::endl;
    }

    {
        std::cout<<"=== Serializer opertator on types ==="<<std::endl;
        bool b = true;
        char c = 'c';
        short int si = 42;
        int i = -42;
        unsigned int ui = 12;
        float f = 42.0;
        double d = -42.0;
        long int li = 12;
        char c_ptr[] = "test c_ptr";
        std::string str = "test str";

        ntw::Serializer ser;
        ser<<b
            <<c
            <<si
            <<i
            <<ui
            <<f
            <<d
            <<li
            <<c_ptr
            <<str;

        std::cout<<ser<<std::endl;

    }

}
