#pragma once

#include <array>
#include <iostream>
#include <variant>

// This is the namespace for all data structures passed between padds
#define Message_Types ScratchPadd::Message_Type::Point, ScratchPadd::Message_Type::Triangle, ScratchPadd::Message_Type::Text, ScratchPadd::Message_Type::Control

namespace ScratchPadd {

    namespace ControlType {
      struct Double {
        double value;
        std::optional<std::pair<double,double>> enforcableRange;
      };
      struct Integer {
        int32_t value;
        std::optional<std::pair<int32_t,int32_t>> enforcableRange;
      };

      struct Boolean {
        bool value;
      };

      struct String {
        std::string value;
        std::optional<std::vector<std::string>> enforcableRange;
      };
    }
  namespace Message_Type {

    #define Control_Types ScratchPadd::ControlType::Double, ScratchPadd::ControlType::Integer, ScratchPadd::ControlType::Boolean, ScratchPadd::ControlType::String
    using ControlVariant = std::variant<Control_Types>;
    struct Control {
      std::string sourceName;
      std::unordered_map<std::string,ControlVariant> controlMap;
    };

    namespace Transform {
      struct Translate {

      };
      struct Scale {

      };
      struct Rotate {

      };
    }

    struct Color {

    };
    struct Point {
      double x{0};
      double y{0};
      friend std::ostream &operator<<( std::ostream &output, const Point &p ) {
        output << "{ X:" << p.x << " Y: " << p.y << " } ";
        return output;
      }
    };

    struct Position : Point {};

    struct Triangle {
      Position position;
      std::array<Point,3> vertexes;
      friend std::ostream &operator<<( std::ostream &output, const Triangle &T ) {
        output << " { position:" << T.position  << ", vertexes: {\n" ;
        for (const Point &vertex: T.vertexes) {
          output << " " << vertex << " ,\n";
        }
        output << " }";
        return output;
      }
    };

    struct Text {
      std::string contents;
      friend std::ostream &operator<<( std::ostream &output, const Text &T ) {
        output << " { text context:" << T.contents << " }";
        return output;
      }
    };
  }

using MessageVariant = std::variant<Message_Types>;
using Message = std::shared_ptr<MessageVariant>;

  template<class Message_Type>
  static Message Make_Msg(Message_Type &&message_type) {
    return std::make_shared<MessageVariant>(message_type);
  }
}