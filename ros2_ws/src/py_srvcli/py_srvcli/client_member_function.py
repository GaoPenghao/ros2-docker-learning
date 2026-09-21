import sys
import rclpy
from rclpy.node import Node
from tutorial_interfaces.srv import AddTwoInts


class MinimalClientAsync(Node):
    def __init__(self):
        super().__init__('minimal_client_async')
        self.cli = self.create_client(AddTwoInts, 'add_two_ints')
        while not self.cli.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('service not available, waiting again...')
        self.req = AddTwoInts.Request()

    def send_request(self, a, b):
        self.req.a = a
        self.req.b = b
        self.future = self.cli.call_async(self.req)
        rclpy.spin_until_future_complete(self, self.future)
        return self.future.result()


def main(args=None):
    rclpy.init(args=args)
    min_cli = MinimalClientAsync()
    response = min_cli.send_request(int(sys.argv[1]), int(sys.argv[2]))
    min_cli.get_logger().info(
        f'Result of add_two_ints: for {sys.argv[1]} + {sys.argv[2]} = {response.sum}')
    min_cli.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
