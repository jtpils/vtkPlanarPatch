#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkPointSource.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkMath.h>
#include <vtkPlane.h>
#include <vtkPlaneSource.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

#include "vtkPlanarPatch.h"

int main(int, char *[])
{
  // Create a point cloud
  double origin[3] = {0,0,0};
  double normal[3] = {1,1,0};
  vtkSmartPointer<vtkPlaneSource> planeSource =
    vtkSmartPointer<vtkPlaneSource>::New();
  planeSource->SetCenter(origin);
  planeSource->SetNormal(normal);
  planeSource->SetXResolution(10);
  planeSource->SetYResolution(10);
  planeSource->Update();

  vtkSmartPointer<vtkPoints> points =
    vtkSmartPointer<vtkPoints>::New();
  // Only keep a subset of the points on the plane
  for(vtkIdType i = 0; i < planeSource->GetOutput()->GetNumberOfPoints(); i++)
    {
    double rand = vtkMath::Random(0,1);
    if(rand < .5)
      {
      double p[3];
      planeSource->GetOutput()->GetPoint(i,p);
      p[0] += vtkMath::Random(-.1,.1);
      p[1] += vtkMath::Random(-.1,.1);
      p[2] += vtkMath::Random(-.1,.1);
      points->InsertNextPoint(p);
      }
    }

  vtkSmartPointer<vtkPolyData> pointsPolyData =
    vtkSmartPointer<vtkPolyData>::New();
  pointsPolyData->SetPoints(points);

  vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter =
    vtkSmartPointer<vtkVertexGlyphFilter>::New();
  glyphFilter->SetInput(pointsPolyData);
  glyphFilter->Update();

  {
  // Write the input
  vtkSmartPointer<vtkXMLPolyDataWriter> writer =
    vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  writer->SetFileName("input.vtp");
  writer->SetInputConnection(glyphFilter->GetOutputPort());
  writer->Write();
  }
  
  vtkSmartPointer<vtkPlane> plane =
    vtkSmartPointer<vtkPlane>::New();
  plane->SetNormal(normal);
  plane->SetOrigin(origin);

  vtkSmartPointer<vtkPlanarPatch> planarPatch =
      vtkSmartPointer<vtkPlanarPatch>::New();
  planarPatch->SetPlane(plane);
  planarPatch->SetFlatOutput(false);
  planarPatch->SetInputConnection(pointsPolyData->GetProducerPort());
  planarPatch->Update();

  // Write the output
  vtkSmartPointer<vtkXMLPolyDataWriter> writer =
    vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  writer->SetFileName("output.vtp");
  writer->SetInputConnection(planarPatch->GetOutputPort());
  writer->Write();
  
  return EXIT_SUCCESS;
}
