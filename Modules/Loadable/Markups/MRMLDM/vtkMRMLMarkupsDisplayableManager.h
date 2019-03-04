/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

#ifndef __vtkMRMLMarkupsDisplayableManager_h
#define __vtkMRMLMarkupsDisplayableManager_h

// MarkupsModule includes
#include "vtkSlicerMarkupsModuleMRMLDisplayableManagerExport.h"

// MarkupsModule/MRMLDisplayableManager includes
#include "vtkMRMLMarkupsDisplayableManagerHelper.h"

// MRMLDisplayableManager includes
#include <vtkMRMLAbstractDisplayableManager.h>

// VTK includes
#include <vtkSlicerAbstractWidget.h>

#include <set>

class vtkMRMLMarkupsNode;
class vtkSlicerViewerWidget;
class vtkMRMLMarkupsDisplayNode;
class vtkAbstractWidget;

/// \ingroup Slicer_QtModules_Markups
class  VTK_SLICER_MARKUPS_MODULE_MRMLDISPLAYABLEMANAGER_EXPORT vtkMRMLMarkupsDisplayableManager :
    public vtkMRMLAbstractDisplayableManager
{
public:

  // Allow the helper to call protected methods of displayable manager
  friend class vtkMRMLMarkupsDisplayableManagerHelper;

  static vtkMRMLMarkupsDisplayableManager *New();
  vtkTypeMacro(vtkMRMLMarkupsDisplayableManager, vtkMRMLAbstractDisplayableManager);
  void PrintSelf(ostream& os, vtkIndent indent) VTK_OVERRIDE;

  /// Check if this is a 2d SliceView displayable manager, returns true if so,
  /// false otherwise. Checks return from GetSliceNode for non null, which means
  /// it's a 2d displayable manager
  virtual bool Is2DDisplayableManager();
  /// Get the sliceNode, if registered. This would mean it is a 2D SliceView displayableManager.
  vtkMRMLSliceNode * GetMRMLSliceNode();

  vtkMRMLMarkupsDisplayableManagerHelper *  GetHelper() { return this->Helper; };

  bool CanProcessInteractionEvent(vtkMRMLInteractionEventData* eventData, double &closestDistance2) VTK_OVERRIDE;
  bool ProcessInteractionEvent(vtkMRMLInteractionEventData* eventData) VTK_OVERRIDE;

  void SetHasFocus(bool hasFocus) VTK_OVERRIDE;
  bool GetGrabFocus() VTK_OVERRIDE;
  bool GetInteractive() VTK_OVERRIDE;

  // Updates markup point preview position.
  // Returns true if the event is processed.
  vtkSlicerAbstractWidget* GetWidgetForPlacement();

  vtkMRMLMarkupsNode* GetActiveMarkupsNodeForPlacement();

  int GetCurrentInteractionMode();

  // Methods from vtkMRMLAbstractSliceViewDisplayableManager

  /// Convert device coordinates (display) to XYZ coordinates (viewport).
  /// Parameter \a xyz is double[3]
  /// \sa ConvertDeviceToXYZ(vtkRenderWindowInteractor *, vtkMRMLSliceNode *, double x, double y, double xyz[3])
  void ConvertDeviceToXYZ(double x, double y, double xyz[3]);

protected:

  vtkMRMLMarkupsDisplayableManager();
  virtual ~vtkMRMLMarkupsDisplayableManager();

  vtkSlicerAbstractWidget* FindClosestWidget(vtkMRMLInteractionEventData *callData, double &closestDistance2);

  virtual void ProcessMRMLNodesEvents(vtkObject *caller, unsigned long event, void *callData) VTK_OVERRIDE;

  /// Wrap the superclass render request in a check for batch processing
  virtual void RequestRender();

  /// Called from RequestRender method if UpdateFromMRMLRequested is true
  /// \sa RequestRender() SetUpdateFromMRMLRequested()
  virtual void UpdateFromMRML() VTK_OVERRIDE;

  virtual void SetMRMLSceneInternal(vtkMRMLScene* newScene) VTK_OVERRIDE;

  /// Called after the corresponding MRML event is triggered, from AbstractDisplayableManager
  /// \sa ProcessMRMLSceneEvents
  virtual void UpdateFromMRMLScene() VTK_OVERRIDE;
  virtual void OnMRMLSceneEndClose() VTK_OVERRIDE;
  virtual void OnMRMLSceneEndImport() VTK_OVERRIDE;
  virtual void OnMRMLSceneNodeAdded(vtkMRMLNode* node) VTK_OVERRIDE;
  virtual void OnMRMLSceneNodeRemoved(vtkMRMLNode* node) VTK_OVERRIDE;

  /// Create a widget.
  vtkSlicerAbstractWidget* CreateWidget(vtkMRMLMarkupsDisplayNode* node);

  /// Called after the corresponding MRML View container was modified
  virtual void OnMRMLDisplayableNodeModifiedEvent(vtkObject* caller) VTK_OVERRIDE;

  /// Handler for specific SliceView actions, iterate over the widgets in the helper
  virtual void OnMRMLSliceNodeModifiedEvent();

  /// Observe the interaction node.
  void AddObserversToInteractionNode();
  void RemoveObserversFromInteractionNode();

  /// Get the widget of a node.
  vtkSlicerAbstractWidget* GetWidget(vtkMRMLMarkupsDisplayNode * node);

  /// Check if it is the right displayManager
  virtual bool IsCorrectDisplayableManager();

  /// Return true if this displayable manager supports(can manage) that node,
  /// false otherwise.
  /// Can be reimplemented to add more conditions.
  /// \sa IsManageable(const char*), IsCorrectDisplayableManager()
  virtual bool IsManageable(vtkMRMLNode* node);
  /// Return true if this displayable manager supports(can manage) that node class,
  /// false otherwise.
  /// Can be reimplemented to add more conditions.
  /// \sa IsManageable(vtkMRMLNode*), IsCorrectDisplayableManager()
  virtual bool IsManageable(const char* nodeClassName);

  /// Focus of this displayableManager is set to a specific markups type when inherited
  std::set<std::string> Focus;

  /// Respond to interactor style events
  virtual void OnInteractorStyleEvent(int eventid) VTK_OVERRIDE;

  /// Accessor for internal flag that disables interactor style event processing
  vtkGetMacro(DisableInteractorStyleEventsProcessing, int);

  vtkMRMLMarkupsDisplayableManagerHelper * Helper;

  double LastClickWorldCoordinates[4];

  vtkMRMLMarkupsNode* CreateNewMarkupsNode(const std::string &markupsNodeClassName);

  vtkWeakPointer<vtkSlicerAbstractWidget> LastActiveWidget;

private:
  vtkMRMLMarkupsDisplayableManager(const vtkMRMLMarkupsDisplayableManager&); /// Not implemented
  void operator=(const vtkMRMLMarkupsDisplayableManager&); /// Not Implemented

  int DisableInteractorStyleEventsProcessing;

  vtkMRMLSliceNode * SliceNode;
};

#endif
