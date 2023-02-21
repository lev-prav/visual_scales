#pragma once
#include <pylon/PylonIncludes.h>
#include <pylon/ImageEventHandler.h>
// Include files used by samples.

// Namespace for using pylon objects.
using namespace Pylon;

// Namespace for using cout.
using namespace std;

//Example of an image event handler.
class CSampleImageEventHandler : public CImageEventHandler
{
public:
    void OnImageGrabbed( CInstantCamera& /*camera*/, const CGrabResultPtr& ptrGrabResult ) override
    {
        cout << "CSampleImageEventHandler::OnImageGrabbed called." << std::endl;

    }
};
