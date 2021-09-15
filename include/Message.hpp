#pragma once

#include <array>
#include <iostream>

// This is the namespace for all data structures passed between padds
#define Message_Types ScratchPadd::Message_Type::Point, ScratchPadd::Message_Type::Triangle, ScratchPadd::Message_Type::Text

namespace ScratchPadd {
  namespace Message_Type {

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