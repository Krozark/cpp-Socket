namespace ntw
{
    int FuncWrapper::getVersion(SocketSerialized& sock)
    {
        sock.clear();
        sock<<NTW_VERSION;
        sock.sendCl();        
    }

    bool FuncWrapper::verifyConnect(SocketSerialized& sock)
    {
        std::cout<<"Envoi du message: <hello!> à la soket "<<sock.id()<<std::endl;
        sock.clear();
        sock<<NTW_WELCOM_MSG;
        sock.sendCl();        
    }

    void FuncWrapper::dispatch(SocketSerialized& request)
    {
        while(request.size()>0)
        {
            std::cout<<request<<std::endl;
            int id = FUNCTONS_ID::UNKNOW;
            request>>id;

            switch(id)
            {
                case FUNCTONS_ID::UNKNOW :
                {
                    std::cerr<<"[ERROR] FuncWrapper::dispatch, FUNCTONS_ID UNKNOW"<<std::endl;
                }break;
                case FUNCTONS_ID::GET_VERSION :
                {
                    getVersion(request);
                }break;
                default:
                    std::cerr<<"[ERROR] FuncWrapper::dispatch, FUNCTONS_ID not find"<<std::endl;

            }
        }
    }
}
