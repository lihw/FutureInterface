// mycontext.cpp
// "Network" example
//
// Copyright 2012 - 2014 Future Interface. All rights reserved.
//
// Future Interface  lihw81@gmail.com
//

#include "mycontext.h"

#include "myscene.h"
#include "myserver.h"
#include "myclient.h"

#include <Paper3D/paper3d.h>

#include <PFoundation/pfoundation.h>


MyContext::MyContext(const PContextProperties &properties, const pchar *networkType)
    : PContext(properties)
{
    m_scene = P_NULL;
}

MyContext::~MyContext()
{
}

pbool MyContext::onInitialized()
{
    m_scene = PNEW(MyScene(this));
    PSceneManager *sceneMgr = module<PSceneManager>("scene-manager");
    sceneMgr->addScene(m_scene);
    sceneMgr->setMainScene(m_scene);

    PNetworkManager *networkMgr = module<PNetworkManager>("network-manager");
    if (m_networkType == "server")
    {
        m_networkNode = PNEW(MyServer("tcp://localhost:6666", networkMgr));
    }
    else 
    {
        m_networkNode = PNEW(MyClient("tcp://localhost:6666", networkMgr));
    }

    return true;
}

pbool MyContext::onUpdate()
{
    return true;
}

void MyContext::onDestroy()
{
}

pbool MyContext::onKeyboard(PEvent *event)
{
    pint32 key = event->parameter(P_EVENTPARAMETER__KEY_SCANCODE).toInt();
    pint32 type = event->getType();
    if (type == P_EVENT__KEYUP)
    {
        switch (key)
        {
            case P_KEY_ESC:
                quit();
                return true; 
        }
    }
    return true;
}

pbool MyContext::onTap(PEvent *event)
{
    if (m_networkType == "client")
    {
        m_networkNode->sendMessage("tap", 3);
    }
    return true;
}
