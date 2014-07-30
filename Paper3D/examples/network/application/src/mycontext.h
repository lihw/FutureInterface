// mycontext.h
// "Network" example
//
// Copyright 2012 - 2014 Future Interface. All rights reserved.
//
// Future Interface  lihw81@gmail.com
//

#ifndef MYCONTEXT_H
#define MYCONTEXT_H

#include <PFoundation/pcontext.h>
#include <PFoundation/pcontextproperties.h>

class PNetworkNode;
class MyScene;

class MyContext : public PContext
{
public:
    MyContext(const PContextProperties &properties, const pchar *networkType);
    ~MyContext();
    
    virtual pbool onInitialized();
    virtual pbool onUpdate();
    virtual void onDestroy();
    

protected:
    virtual pbool onKeyboard(PEvent *event);
    virtual pbool onTap(PEvent *event);

private:
    MyScene      *m_scene;
    PString       m_networkType;
    PNetworkNode *m_networkNode;
};

#endif
