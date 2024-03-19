import vtk
import numpy as np
import sys

def plot_obj(obj_path, color, opacity):
    # Read the obj file
    reader = vtk.vtkOBJReader()
    reader.SetFileName(obj_path)
    reader.Update()

    # Create a mapper
    mapper = vtk.vtkPolyDataMapper()
    mapper.SetInputConnection(reader.GetOutputPort())

    # Create an actor
    actor = vtk.vtkActor()
    actor.SetMapper(mapper)
    actor.GetProperty().SetColor(color)
    actor.GetProperty().SetOpacity(opacity)

    return actor

def main(obj_path:str='../build/triangulation.obj'):
    # Create a renderer
    renderer = vtk.vtkRenderer()
    renderer.SetBackground(0.1, 0.2, 0.4)

    # Create a render window
    render_window = vtk.vtkRenderWindow()
    render_window.AddRenderer(renderer)

    # Create a render window interactor
    render_window_interactor = vtk.vtkRenderWindowInteractor()
    render_window_interactor.SetRenderWindow(render_window)

    # Add the actors to the scene
    # obj_path = '../build/triangulation.obj'
    color = np.array([0.5, 0.5, 0.5])
    opacity = 1.0
    actor = plot_obj(obj_path, color, opacity)
    renderer.AddActor(actor)

    # Start the interaction
    render_window.Render()
    render_window_interactor.Start()

if __name__ == '__main__':
    path = sys.argv[1] if len(sys.argv) > 1 else '../build/triangulation.obj'
    main(path)