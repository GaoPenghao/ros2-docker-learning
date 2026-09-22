import rclpy
from rclpy.node import Node
from tutorial_interfaces.srv import AddTwoInts


class MinimalService(Node):
    def __init__(self):
        super().__init__("minimal_service")
        self.srv = self.create_service(
            AddTwoInts, "add_two_ints", self.add_two_ints_callback
        )

    def add_two_ints_callback(self, request, response):
        response.sum = request.a + request.b
        self.get_logger().info(f"Incoming request\na: {request.a} b: {request.b}")
        return response


def main(args=None):
    rclpy.init(args=args)
    min_srv = MinimalService()
    rclpy.spin(min_srv)
    rclpy.shutdown()


if __name__ == "__main__":
    main()
