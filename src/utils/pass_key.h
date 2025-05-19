#pragma once

namespace game
{

/**
 * PassKey is a class that is used to restrict access to an API. Only the type specified as the template parameter can
 * construct the object.
 */
template <class T>
class PassKey
{
    friend T;
    PassKey() = default;
};

}
