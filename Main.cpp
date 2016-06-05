# include <Siv3D.hpp>

void Main()
{
	Webcam webcam(0);

	if (!webcam.start())
	{
		return;
	}

	Image image;
	DynamicTexture texture;

	const size_t frame = 50;

	Array<std::array<double, frame>> data(640 * 480);

	for (auto& elem : data)
	{
		elem.fill(0);
	}

	uint32 insertIndex = 0;

	while (System::Update())
	{
		if (webcam.hasNewFrame())
		{
			webcam.getFrame(image);

			image.gaussianBlur(2, 2);

			++insertIndex %= frame;

			{
				const Color* p = image[0];
				auto* pData = &data[0];
					
				for (int i = 0; i < 640 * 480; ++i)
				{
					(*pData)[insertIndex] = p->grayscaleF();
				
					++p;
					++pData;
				}
			}

			{
				Color* p = image[0];

				for (int i = 0; i < 640 * 480; ++i)
				{
					const auto& buf = data[i];
			
					double sum = 0.0;
			
					for (const auto& value : buf)
					{
						sum += value;
					}

					const double f = (sum / frame - buf[insertIndex]);
			
					*p++ = ColorF(0.5 - f * 8.0);
				}
			}

			texture.fill(image);
		}

		texture.draw();
	}
}
