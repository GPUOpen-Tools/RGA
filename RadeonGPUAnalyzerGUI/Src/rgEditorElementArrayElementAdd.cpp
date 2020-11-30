// C++.
#include <cassert>

// Local.
#include <RadeonGPUAnalyzerGUI/Include/Qt/rgEditorElementArrayElementAdd.h>
#include <RadeonGPUAnalyzerGUI/Include/Qt/rgPipelineStateEditorWidgetArrayElementAdd.h>

rgEditorElementArrayElementAdd::rgEditorElementArrayElementAdd(QWidget* pParent, const std::string& memberName, std::function<void(int)> elementRemovedCallback)
    : rgEditorElement(pParent, memberName, rgEditorDataType::Array)
    , m_elementRemovedCallback(elementRemovedCallback)
{
    m_pEditorWidget = new rgPipelineStateEditorWidgetArrayElementAdd(this);
    assert(m_pEditorWidget != nullptr);
    if (m_pEditorWidget != nullptr)
    {
        ui.editorLayout->insertWidget(0, m_pEditorWidget);

        // Connect the loss of focus handler.
        bool isConnected = connect(m_pEditorWidget, &rgPipelineStateEditorWidget::FocusOutSignal,
            this, &rgEditorElement::HandleEditorFocusOut);
        assert(isConnected);
    }
}

rgPipelineStateEditorWidget* rgEditorElementArrayElementAdd::GetEditorWidget()
{
    return m_pEditorWidget;
}

void rgEditorElementArrayElementAdd::AddNewElement()
{
    assert(m_pArraySizeElement != nullptr);
    if (m_pArraySizeElement != nullptr)
    {
        // Is there a maximum size specified for this array?
        bool maximumSizeReached = false;
        uint32_t currentArraySize = m_pArraySizeElement->GetValue();
        if (m_maximumSize > 0)
        {
            maximumSizeReached = currentArraySize == static_cast<uint32_t>(m_maximumSize);
        }

        // Increase the array dimension if possible.
        if (!maximumSizeReached)
        {
            m_pArraySizeElement->SetValue(currentArraySize + 1);
        }
    }
}

void rgEditorElementArrayElementAdd::RemoveElement(int elementIndex)
{
    // Remove the given element, and shift all remaining elements down in the array.
    assert(m_elementRemovedCallback != nullptr);
    if (m_elementRemovedCallback != nullptr)
    {
        // Invoke the callback used to handle elements being removed from the array.
        m_elementRemovedCallback(elementIndex);
    }

    // Update the array size element.
    int currentArraySize = static_cast<int>(m_pArraySizeElement->GetValue());
    m_pArraySizeElement->SetValue(currentArraySize - 1);

    // Invoke the resized callback.
    InvokeElementResizedCallback();
}

void rgEditorElementArrayElementAdd::InvokeElementResizedCallback()
{
    // Invoke the array resized callback if available.
    if (m_arrayResizedCallback != nullptr)
    {
        m_arrayResizedCallback();
    }

    // If the array was resized, and no longer has any child elements, update the indentation.
    if (m_childItems.empty())
    {
        UpdateIndentation();
    }
}

rgEditorElementNumeric<uint32_t>* rgEditorElementArrayElementAdd::GetArraySizeElement() const
{
    return m_pArraySizeElement;
}

void rgEditorElementArrayElementAdd::SetArraySizeElement(rgEditorElementNumeric<uint32_t>* pArraySizeElement)
{
    assert(pArraySizeElement != nullptr);
    if (pArraySizeElement != nullptr)
    {
        m_pArraySizeElement = pArraySizeElement;
    }
}

void rgEditorElementArrayElementAdd::SetMaximumArraySize(uint32_t maximumSize)
{
    m_maximumSize = maximumSize;
}

void rgEditorElementArrayElementAdd::SetResizeCallback(std::function<void()> resizeCallback)
{
    assert(resizeCallback != nullptr);
    if (resizeCallback != nullptr)
    {
        m_arrayResizedCallback = resizeCallback;
    }
}