// Let's play with JS6 classes :)

LSystem = function () {

    // Private scope outside of this anonymous func
    var vecLen = 2;
    
    class Renderer {
        constructor(doc, material) {
            this.material = material;
            this.doc = doc;
            this.action = {
                'F': this.fwd,
                'f': this.hop,
                '-': this.left,
                '+': this.right,
                '[': this.pushState,
                ']': this.popState
            }
        }

        fwd(parent) {
            let v = parent.clone();
            v.y += vecLen;
            return v;
        }

        hop(parent) {

        }

        left(parent) {
            var axis = new THREE.Vector3(1, 0, 0);
            var angle = Math.PI / 3;

            let v = parent.clone();
            v.y += vecLen;
            v.applyAxisAngle(axis, angle);
            return v;
        }

        right(parent) {
            var axis = new THREE.Vector3(1, 0, 0);
            var angle = Math.PI / 3;
            
            let v = parent.clone();
            v.y += vecLen;
            v.applyAxisAngle(axis, angle);
            return v;
        }

        pushState(parent) {

        }

        popState(parent) {

        }
        
        draw(scene) {
            // Create vertices
            var root = new THREE.Vector3(),
                prev = root;

            for (let elem of this.doc) {
                let v = this.action[elem](prev);
                let g = new THREE.Geometry();
                g.vertices.push(prev, v);
                let l = new THREE.Line(g, this.material);
                scene.add(l);
                prev = v;
            }
        }
    }
   
    return Renderer;
}();
