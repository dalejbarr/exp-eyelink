#ifndef EXP_DEVICE_INCLUDED
#define EXP_DEVICE_INCLUDED
#define EXP_DEVICE_DISPLAY 1

#include <boost/smart_ptr.hpp>
#include <map>

class Device {
public:
  long m_id;
  Device(long id);
  virtual ~Device();
  virtual long GetID() { return(m_id); };
};

typedef boost::shared_ptr<Device> DevicePtr;
typedef std::multimap<long, DevicePtr> DeviceMMap;
typedef std::pair<long, DevicePtr> DevPair;

#endif
