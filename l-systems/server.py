# Server interface for JS to build l-system documents

from SimpleHTTPServer import SimpleHTTPRequestHandler, BaseHTTPServer
import json
import logging
import model

class RequestHandler(SimpleHTTPRequestHandler):
    log = logging.getLogger(__name__)

    def do_GET(self, requestData=None):
        try:
            if self.path.endswith("api"):
                self.log.info("handling api req: %s, %s" %(self.path, requestData))
                self.send_response(200)
                self.send_header("Content-type", "text/json")
                self.end_headers()

                rule = json.loads(requestData)
                generations = int(rule['generations'])
                rule = model.Program(generations, 25.7, 'F', [model.Rule('F', rule['rule'])])
                gen = model.NodeGenerate()
                doc = gen(rule, generations)

                self.wfile.write(doc)
                return
            else:
                self.log.info("standard req: %s" %self.path)
                return SimpleHTTPRequestHandler.do_GET(self)
 
        except Exception, e:
            self.log.exception(e)
            self.send_error(404, "File Not Found %s" % self.path)
 
    def do_POST(self):
        contentLen = int(self.headers.getheader('content-length', 0))
        requestData = self.rfile.read(contentLen)
        return self.do_GET(requestData)


PORT = 8080

def main():
    try:
        logging.basicConfig()
        logging.getLogger().setLevel(logging.INFO)

        # you can specify any port you want by changing <q>81</q>
        server = BaseHTTPServer.HTTPServer(("", PORT), RequestHandler)
        server.serve_forever()
    except KeyboardInterrupt:
        server.socket.close()

if __name__ == '__main__':
    main()
