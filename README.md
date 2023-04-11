# Particles
A physics collision simulation featuring particles interacting in a two-dimensional box. Project aims to utilize graphical components, mathematical logic, and object oriented programing to produce an effective model with elastic collisions. Project intends to simulate chemical kinetics after development of the initial collision simulation.

## Project Objects
### Vector2D
Class maintains information about x and y vector components and provides relevant procedures. All functions and members are public.
- float x, y;
- float magnitude() : returns the magnitude of the vector through use of the Pythagorean theorem
- Vector2D operator+(const Vector2D& vec) : adds the current vector to the inputted one and returns the result
- Vector2D operator-(const Vector2D& vec) : subtracts the inputted vector from the current one and returns the result
- float operator*(const Vector2D& vec) : returns the scalar produced by the dot product of inputted and current vectors

### Particle
Class maintains information about a particle.
- Vector2D p : position
- Vector2D v : velocity
- Vector2D a : acceleration; may be unnecessary, as all particles are to experience no acceleration
- float m : mass
- float r : radius 
- ID2D1SolidColorBrush c : colour
- void Paint(ID2D1HwndRenderTarget* renderTarget) : displays the particle at it's coordinates as a filled dot of radius r and colour c

### System
Class maintains information about the system and the box or enclosed space containing the particles simulated. Future expansions regarding reaction kinetics rely solely on additions to this class.
- Vector2D tl : top left corner of enclosed space
- Vector2D br : bottom right corner of enclosed space; may be considered the width and height of the space
- Particle** particles : an array of particles within the enclosed space
- Particle** BroadDetect() : uses sweep and prune algorithm to detect possible collisions; returns all possible collisions
  1. project all particles onto the x-axis and sort them such that they are in order of closest to furthest from x = 0
  2. maintain current active intervals; iterate over the collection of ordered particles where, if the particles are overlapping (use invervals), they are added to the active interval, and if they are not, then the one currently in the active interval is removed; if the active interval is emptied before the current particle is added, it is automatically added
  3. check the intersection of particles with active intervals; this algorithm will not miss any possible collisions

- Particle** NarrowDetect(Particle** collisions) : checks each possible particle/particle collision to see if it occurs; checks through comparing the distance between their centres (through use of the pythagorean theorem), to the sum of their radii, and if it is less they are colliding
- void ParticleCollide(Particle* p1, Particle* p2) : solves the collision of two particles; these particles may be overlapping, and thus a method similar to BorderCollide() where linear interpolation is used may be employed in reverse, such that the position prior to the collision may be determined for each -> this is when the distance between the two is exactly equal to the sum of their radii; through use of a fancy progression of steps that can be found in the following link, the velocities of the two particles involved may be determined: https://www.vobarian.com/collisions/2dcollisions2.pdf; note that the unit vector may be implemented as a function of Vector2D if seen as necessary 
- void BorderCollide() : runs through all particles to check if they are colliding with one of the borders of the enclosed space; note: does not need to be heavily optimized as the function is not too computationally heavy
  1. check for the collision before updating the position
  2. if the collision does occur, use linear interpolation to determine the exact coordinates of the particle upon colliding using a parametric equation; x(t) = t * x(1) + (1 - t) * x(0), and y(t) = t * y(1) + (1 - t) * y(0) for some point p(x(t), y(t))
  3. a horizontal collision occurs when y(tc) = br.y - r or tl.y + r, whilst a verticle one occurs when x(tc) = br.x - r or tl.x + r; regardless tc can be solved for such that the point p(x(tc), y(tc)) is known, after which the velocity can be multiplied by -1 in the component that it must change, such that the final position of the frame can be determined
  4. tc = ((br.x - r) - y(0))/(y(1) - y(0)), for instance
- void Paint() : paints the enclosed space the particles are found in; in future implementations, other features such as temperature may be displayed by this class

### Engine
A class developed prior to this project that permits usage of Direct2D to create a graphical component to the application. Primarily, there are two functions and one member that are to be used from this class:
- float dt : the change in time per frame
- HRESULT Engine::OnRender() : the paint function called every frame to update the graphical interface
- void Engine::Update(ID2D1HwndRenderTarget* renderTarget) : the processing function called every frame to update project logic; here the position is updated each frame by multiplication with dt
