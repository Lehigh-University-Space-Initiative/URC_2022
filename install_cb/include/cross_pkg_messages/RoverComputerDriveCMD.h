// Generated by gencpp from file cross_pkg_messages/RoverComputerDriveCMD.msg
// DO NOT EDIT!


#ifndef CROSS_PKG_MESSAGES_MESSAGE_ROVERCOMPUTERDRIVECMD_H
#define CROSS_PKG_MESSAGES_MESSAGE_ROVERCOMPUTERDRIVECMD_H


#include <string>
#include <vector>
#include <memory>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <std_msgs/Header.h>
#include <geometry_msgs/Vector3.h>

namespace cross_pkg_messages
{
template <class ContainerAllocator>
struct RoverComputerDriveCMD_
{
  typedef RoverComputerDriveCMD_<ContainerAllocator> Type;

  RoverComputerDriveCMD_()
    : header()
    , value()  {
    }
  RoverComputerDriveCMD_(const ContainerAllocator& _alloc)
    : header(_alloc)
    , value(_alloc)  {
  (void)_alloc;
    }



   typedef  ::std_msgs::Header_<ContainerAllocator>  _header_type;
  _header_type header;

   typedef  ::geometry_msgs::Vector3_<ContainerAllocator>  _value_type;
  _value_type value;





  typedef boost::shared_ptr< ::cross_pkg_messages::RoverComputerDriveCMD_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::cross_pkg_messages::RoverComputerDriveCMD_<ContainerAllocator> const> ConstPtr;

}; // struct RoverComputerDriveCMD_

typedef ::cross_pkg_messages::RoverComputerDriveCMD_<std::allocator<void> > RoverComputerDriveCMD;

typedef boost::shared_ptr< ::cross_pkg_messages::RoverComputerDriveCMD > RoverComputerDriveCMDPtr;
typedef boost::shared_ptr< ::cross_pkg_messages::RoverComputerDriveCMD const> RoverComputerDriveCMDConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::cross_pkg_messages::RoverComputerDriveCMD_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::cross_pkg_messages::RoverComputerDriveCMD_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::cross_pkg_messages::RoverComputerDriveCMD_<ContainerAllocator1> & lhs, const ::cross_pkg_messages::RoverComputerDriveCMD_<ContainerAllocator2> & rhs)
{
  return lhs.header == rhs.header &&
    lhs.value == rhs.value;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::cross_pkg_messages::RoverComputerDriveCMD_<ContainerAllocator1> & lhs, const ::cross_pkg_messages::RoverComputerDriveCMD_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace cross_pkg_messages

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsFixedSize< ::cross_pkg_messages::RoverComputerDriveCMD_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::cross_pkg_messages::RoverComputerDriveCMD_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::cross_pkg_messages::RoverComputerDriveCMD_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::cross_pkg_messages::RoverComputerDriveCMD_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::cross_pkg_messages::RoverComputerDriveCMD_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::cross_pkg_messages::RoverComputerDriveCMD_<ContainerAllocator> const>
  : TrueType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::cross_pkg_messages::RoverComputerDriveCMD_<ContainerAllocator> >
{
  static const char* value()
  {
    return "cb1dd5548a7b2dd376207dab58ce4eff";
  }

  static const char* value(const ::cross_pkg_messages::RoverComputerDriveCMD_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xcb1dd5548a7b2dd3ULL;
  static const uint64_t static_value2 = 0x76207dab58ce4effULL;
};

template<class ContainerAllocator>
struct DataType< ::cross_pkg_messages::RoverComputerDriveCMD_<ContainerAllocator> >
{
  static const char* value()
  {
    return "cross_pkg_messages/RoverComputerDriveCMD";
  }

  static const char* value(const ::cross_pkg_messages::RoverComputerDriveCMD_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::cross_pkg_messages::RoverComputerDriveCMD_<ContainerAllocator> >
{
  static const char* value()
  {
    return "Header header\n"
"geometry_msgs/Vector3 value\n"
"================================================================================\n"
"MSG: std_msgs/Header\n"
"# Standard metadata for higher-level stamped data types.\n"
"# This is generally used to communicate timestamped data \n"
"# in a particular coordinate frame.\n"
"# \n"
"# sequence ID: consecutively increasing ID \n"
"uint32 seq\n"
"#Two-integer timestamp that is expressed as:\n"
"# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')\n"
"# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')\n"
"# time-handling sugar is provided by the client library\n"
"time stamp\n"
"#Frame this data is associated with\n"
"string frame_id\n"
"\n"
"================================================================================\n"
"MSG: geometry_msgs/Vector3\n"
"# This represents a vector in free space. \n"
"# It is only meant to represent a direction. Therefore, it does not\n"
"# make sense to apply a translation to it (e.g., when applying a \n"
"# generic rigid transformation to a Vector3, tf2 will only apply the\n"
"# rotation). If you want your data to be translatable too, use the\n"
"# geometry_msgs/Point message instead.\n"
"\n"
"float64 x\n"
"float64 y\n"
"float64 z\n"
;
  }

  static const char* value(const ::cross_pkg_messages::RoverComputerDriveCMD_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::cross_pkg_messages::RoverComputerDriveCMD_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.header);
      stream.next(m.value);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct RoverComputerDriveCMD_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::cross_pkg_messages::RoverComputerDriveCMD_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::cross_pkg_messages::RoverComputerDriveCMD_<ContainerAllocator>& v)
  {
    s << indent << "header: ";
    s << std::endl;
    Printer< ::std_msgs::Header_<ContainerAllocator> >::stream(s, indent + "  ", v.header);
    s << indent << "value: ";
    s << std::endl;
    Printer< ::geometry_msgs::Vector3_<ContainerAllocator> >::stream(s, indent + "  ", v.value);
  }
};

} // namespace message_operations
} // namespace ros

#endif // CROSS_PKG_MESSAGES_MESSAGE_ROVERCOMPUTERDRIVECMD_H