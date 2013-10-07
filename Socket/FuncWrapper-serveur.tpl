namespace ntw
{
    int FuncWrapper::getVersion(SocketSerialized& sock)
    {
        sock.clear();
        sock<<42;
        sock.send();
    }

    void FuncWrapper::dispatch(SocketSerialized& request)
    {
        if(request.size()>0)
        {
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
