#pragma once
#ifndef _IF_ICOMM_HPP
#define _IF_ICOMM_HPP

class IComm
{
public:
    virtual errno_t Open() = 0;
    virtual void Close() = 0;
    virtual bool IsConnected() const = 0;
    virtual void registerCallback(std::function<int(void *, void *)> cb) = 0;
    virtual int SendData(const char *ptr_data, size_t length) = 0;
};

#endif